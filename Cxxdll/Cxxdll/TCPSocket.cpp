#include "pch.h"

CTCPSocketManage::CTCPSocketManage() :
	m_running(false),
	m_pRecvDataLine(new CDataLine),
	m_pSendDataLine(new CDataLine),
	m_eventBaseCfg(event_config_new()),
	m_iServiceType(ServiceType::SERVICE_TYPE_END),
	m_ConnectServerBase(nullptr),
	m_socket(INVALID_SOCKET),
	m_port(0),
	m_ip(""),
	m_timerCnt(0)
{
#ifdef _WIN32
	WSADATA wsa;
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		std::cout << "Init socket dll err" << std::endl;
	}
	if (event_config_set_flag(m_eventBaseCfg, EVENT_BASE_FLAG_STARTUP_IOCP))
	{
		std::cout << "Init iocp is err" << std::endl;
	}
	if (evthread_use_windows_threads() != 0)
	{
		std::cout << "Init iocp thread is err" << std::endl;
	}
	if (event_config_set_num_cpus_hint(m_eventBaseCfg, si.dwNumberOfProcessors) != 0)
	{
		std::cout << "Set the number of CPU is err" << std::endl;
	}
#elif linux 
	if (evthread_use_pthreads() != 0)
	{
		std::cout << "Init iocp thread is err" << std::endl;
	}
#endif
}

CTCPSocketManage::~CTCPSocketManage()
{
	WSACleanup();
}

bool CTCPSocketManage::Stop()
{
	std::cout << "service tcp stop begin" << std::endl;

	if (!m_running)
	{
		std::cout << "TCPSocketManage is not running" << std::endl;
		return false;
	}

	m_running = false;

	event_base_loopbreak(m_ConnectServerBase);
	
	std::cout << "service tcp stop end" << std::endl;

	return true;
}

int CTCPSocketManage::GetTimerCnt()
{
	return m_timerCnt;
}

bool CTCPSocketManage::InitNetwork(char* ip, int port, int timerCnt)
{
	m_port = port;
	m_timerCnt = timerCnt;

	if (timerCnt < 0 || timerCnt > MAX_TIMER_THRED_NUMS)
	{
		m_timerCnt = 1;
	}
	if (ip && strlen(ip) < sizeof(m_ip))
	{
		strcpy(m_ip, ip);
	}

	return true;
}


bool CTCPSocketManage::Start()
{
	if (m_running == true)
	{
		std::cout << "service tcp already have been running" << std::endl;
		return false;
	}
	m_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_socket < 0)
	{
		std::cout << "socket is err" << std::endl;
		return false;
	}

	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(m_port);
	sin.sin_addr.S_un.S_addr = inet_addr(m_ip);
	if (connect(m_socket, (sockaddr*)&sin, sizeof(sockaddr_in)) < 0)
	{
		std::cout << "connect is err" << std::endl;
		return false;
	}

	m_iServiceType = ServiceType::SERVICE_TYPE_CLIENT;
	m_running = true;

	m_socketThread.push_back(new std::thread(&CTCPSocketManage::ThreadSendMsgThread, this));
	m_socketThread.push_back(new std::thread(&CTCPSocketManage::ConnectServerThread, this, std::ref(m_socket)));

	return true;
}

bool CTCPSocketManage::ConnectServer(SOCKFD& fd)
{
	m_ConnectServerBase = event_base_new_with_config(m_eventBaseCfg);
	event_config_free(m_eventBaseCfg);

	if (!m_ConnectServerBase)
	{
		std::cout << "TCP Could not initialize libevent!" << std::endl;
		return false;
	}
	struct bufferevent* bev = bufferevent_socket_new(m_ConnectServerBase, fd, /*BEV_OPT_CLOSE_ON_FREE | */BEV_OPT_THREADSAFE);
	if (!bev)
	{
		std::cout << "Error constructing bufferevent!" << std::endl;
		return false;
	}

	// 设置应用层收发数据包，单次大小
	SetMaxSingleReadAndWrite(bev, SOCKET_RECV_BUF_SIZE, SOCKET_SEND_BUF_SIZE);

	// 添加事件，并设置好回调函数
	bufferevent_setcb(bev, ReadCB, nullptr, EventCB, (void*)this);
	if (bufferevent_enable(bev, EV_READ | EV_WRITE) < 0)
	{
		std::cout << "add event fail!!!" << std::endl;
		bufferevent_free(bev);
		return false;
	}

	// 设置读超时，当做心跳。网关服务器才需要
	if (m_iServiceType == ServiceType::SERVICE_TYPE_LOGIC)
	{
		timeval tvRead;
		tvRead.tv_sec = CHECK_HEAETBEAT_SECS * KEEP_ACTIVE_HEARTBEAT_COUNT;
		tvRead.tv_usec = 0;
		bufferevent_set_timeouts(bev, &tvRead, nullptr);
	}
	
	// 保存信息
	m_socketInfo.bev = bev;
	m_socketInfo.isConnect = true;
	if (!m_socketInfo.lock)
	{
		m_socketInfo.lock = new std::mutex;
	}

	std::cout << "Socket connection succeeded" << std::endl;
	return true;
}

void CTCPSocketManage::ConnectServerThread(SOCKFD& fd)
{
	if (!ConnectServer(std::ref(m_socket)))
	{
		m_running = false;
		return;
	}

	event_base_dispatch(m_ConnectServerBase);
	event_base_free(m_ConnectServerBase);
}

void CTCPSocketManage::ReadCB(bufferevent* bev, void* data)
{
	if (!data || !bev)
	{
		return;
	}
	((CTCPSocketManage*)data)->RecvData(bev);
}

bool CTCPSocketManage::RecvData(bufferevent* bev)
{
	if (bev == nullptr)
	{
		std::cout << "RecvData error bev == nullptr" << std::endl;
		return false;
	}

	struct evbuffer* input = bufferevent_get_input(bev);

	size_t maxSingleRead = Min_(evbuffer_get_length(input), SOCKET_RECV_BUF_SIZE);

	std::unique_ptr<char[]> recvBuf(new char[maxSingleRead]);

	size_t realAllSize = evbuffer_copyout(input, recvBuf.get(), maxSingleRead);
	if (realAllSize <= 0)
	{
		return false;
	}

	// 剩余处理数据
	size_t handleRemainSize = realAllSize;

	// 解出包头
	NetMessageHead* pNetHead = (NetMessageHead*)recvBuf.get();

	// 错误判断
	if (handleRemainSize >= sizeof(NetMessageHead) && pNetHead->uMessageSize > SOCKET_RECV_BUF_SIZE)
	{
		// 消息格式不正确
		std::cout << "消息格式不正确" << std::endl;
		return false;
	}

	// 粘包处理
	while (handleRemainSize >= sizeof(NetMessageHead) && handleRemainSize >= pNetHead->uMessageSize)
	{
		unsigned int messageSize = pNetHead->uMessageSize;
		if (messageSize > MAX_TEMP_SENDBUF_SIZE)
		{
			// 消息格式不正确
			std::cout << "消息格式不正确" << std::endl;
			return false;
		}

		int realSize = messageSize - sizeof(NetMessageHead);
		if (realSize < 0)
		{
			// 数据包不够包头
			std::cout << "数据包不够包头" << std::endl;
			return false;
		}

		void* pData = nullptr;
		if (realSize > 0)
		{
			// 没数据就为nullptr
			pData = (void*)(recvBuf.get() + realAllSize - handleRemainSize + sizeof(NetMessageHead));
		}

		// 派发数据
		DispatchPacket(bev, pNetHead, pData, realSize);

		handleRemainSize -= messageSize;

		pNetHead = (NetMessageHead*)(recvBuf.get() + realAllSize - handleRemainSize);
	}

	evbuffer_drain(input, realAllSize - handleRemainSize);

	return true;
}

bool CTCPSocketManage::DispatchPacket(void* pBufferevent, NetMessageHead* pHead, void* pData, int size)
{
	if (!pBufferevent || !pHead)
	{
		return false;
	}
	if (pHead->uMainID == 1) //心跳包
	{
		return true;
	}
	CDataLine* pDataLine = GetRecvDataLine();
	if (!pDataLine)
	{
		return false;
	}

	SocketReadLine msg;
	msg.uHandleSize = size;
	msg.pBufferevent = pBufferevent;
	msg.netMessageHead = *pHead;

	std::unique_ptr<char[]> uniqueBuf(new char[size + sizeof(SocketReadLine)]);
	memcpy(uniqueBuf.get(), &msg, sizeof(SocketReadLine));
	memcpy(uniqueBuf.get() + sizeof(SocketReadLine), pData, size);

	if (pDataLine->AddData(uniqueBuf.get(), size + sizeof(SocketReadLine), SysMsgCmd::HD_SOCKET_READ) == 0)
	{
		return false;
	}

	return true;
}

//网络关闭处理
bool CTCPSocketManage::OnSocketCloseEvent(unsigned long uAccessIP, unsigned int uIndex, unsigned int uConnectTime, unsigned char socketType)
{
	SocketCloseLine SocketClose;
	SocketClose.uConnectTime = uConnectTime;
	SocketClose.uIndex = uIndex;
	SocketClose.uAccessIP = uAccessIP;
	SocketClose.socketType = socketType;
	return (m_pRecvDataLine->AddData(&SocketClose, sizeof(SocketClose), SysMsgCmd::HD_SOCKET_CLOSE) != 0);
}

CDataLine* CTCPSocketManage::GetRecvDataLine()
{
	return m_pRecvDataLine;
}

CDataLine* CTCPSocketManage::GetSendDataLine()
{
	return m_pSendDataLine;
}

void CTCPSocketManage::RemoveTCPSocketStatus(bool isClientAutoClose/* = false*/)
{
	// 加锁
	m_ConditionVariable.GetMutex().lock();
	if (!m_socketInfo.isConnect)
	{
		return;
	}
	if (!m_socketInfo.lock)
	{
		m_socketInfo.lock = new std::mutex;
	}
	// 和发送线程相关的锁
	m_socketInfo.lock->lock();

	m_socketInfo.isConnect = false;
	bufferevent_free(m_socketInfo.bev);
	m_socketInfo.bev = nullptr;

	// 解锁发送线程
	m_socketInfo.lock->unlock();

	// 解锁多线程
	m_ConditionVariable.GetMutex().unlock();
}

void CTCPSocketManage::SetTcpRcvSndBUF(SOCKFD fd, int rcvBufSize, int sndBufSize)
{
	int optval = 0;
	int optLen = sizeof(int);

	getsockopt(fd, SOL_SOCKET, SO_RCVBUF, (char*)&optval, &optLen);
	if (optval < rcvBufSize * 2)
	{
		optval = rcvBufSize;
		setsockopt(fd, SOL_SOCKET, SO_RCVBUF, (char*)&optval, sizeof(optval));
	}

	getsockopt(fd, SOL_SOCKET, SO_SNDBUF, (char*)&optval, &optLen);
	if (optval < sndBufSize * 2)
	{
		optval = sndBufSize;
		setsockopt(fd, SOL_SOCKET, SO_SNDBUF, (char*)&optval, sizeof(optval));
	}
}

void CTCPSocketManage::SetMaxSingleReadAndWrite(bufferevent* bev, int rcvBufSize, int sndBufSize)
{
	if (bufferevent_get_max_single_read(bev) < rcvBufSize && bufferevent_set_max_single_read(bev, rcvBufSize) < 0)
	{
		std::cout << "bufferevent_set_max_single_read fail" << std::endl;
	}

	/*if (bufferevent_set_max_single_write(bev, sndBufSize) < 0)
	{
		COUT_LOG(LOG_CERROR, "bufferevent_set_max_single_write fail,fd=%d", fd);
	}*/
}

void CTCPSocketManage::EventCB(bufferevent* bev, short events, void* data)
{
	((CTCPSocketManage*)data)->RemoveTCPSocketStatus(true);
}

bool CTCPSocketManage::SendData(const char* pData, size_t size, int mainID, int assistID, int handleCode, void* pBufferevent, unsigned int uIdentification/* = 0*/)
{
	if (!pBufferevent)
	{
		std::cout << "!pBufferevent" << std::endl;
		return false;
	}

	if (size < 0 || size > MAX_TEMP_SENDBUF_SIZE - sizeof(NetMessageHead))
	{
		std::cout << "invalid message size size = " << size << std::endl;
		return false;
	}

	// 整合一下数据
	std::unique_ptr<char[]> SendBuf(new char[sizeof(SendDataLineHead) + sizeof(NetMessageHead) + size]);

	// 拼接包头
	NetMessageHead* pHead = reinterpret_cast<NetMessageHead*>((char*)SendBuf.get() + sizeof(SendDataLineHead));
	pHead->uMainID = (unsigned int)mainID;
	pHead->uAssistantID = assistID;
	pHead->uMessageSize = (unsigned int)(sizeof(NetMessageHead) + size);
	pHead->uHandleCode = handleCode;
	pHead->uIdentification = uIdentification;

	// 包体
	if (pData && size > 0)
	{
		memcpy(SendBuf.get() + sizeof(SendDataLineHead) + sizeof(NetMessageHead), pData, size);
	}

	// 投递到发送队列
	if (m_pSendDataLine)
	{
		SendDataLineHead* pLineHead = reinterpret_cast<SendDataLineHead*>(SendBuf.get());
		pLineHead->dataLineHead.uSize = pHead->uMessageSize;
		pLineHead->pBufferevent = pBufferevent;

		unsigned int addBytes = m_pSendDataLine->AddData(pLineHead, sizeof(SendDataLineHead) + pHead->uMessageSize);

		if (addBytes == 0)
		{
			std::cout << "投递消息失败,assistID" << assistID << std::endl;
			return false;
		}
	}

	return true;
}

TCPSocketInfo& CTCPSocketManage::GetTCPSocketInfo()
{
	return m_socketInfo;
}

std::vector<std::thread*>& CTCPSocketManage::GetSockeThreadVec()
{
	return m_socketThread;
}

bool& CTCPSocketManage::GetRuninged()
{
	return m_running;
}

ConditionVariable& CTCPSocketManage::GetConditionVariable()
{
	return m_ConditionVariable;
}

void CTCPSocketManage::HandleSendData(ListItemData* pListItem)
{
	if (!pListItem)
	{
		return;
	}
	if (pListItem->stDataHead.uSize == 0 || pListItem->pData == nullptr)
	{
		return;
	}
	SendDataLineHead* pSocketSend = reinterpret_cast<SendDataLineHead*>(pListItem->pData);
	unsigned int size = pSocketSend->dataLineHead.uSize;
	void* pData = pListItem->pData + sizeof(SendDataLineHead);

	if (!m_socketInfo.lock)
	{
		return;
	}
	{
		std::lock_guard<std::mutex> guard(*m_socketInfo.lock);
		if (!m_socketInfo.isConnect || !m_socketInfo.bev)
		{
			return;
		}
		if (bufferevent_write(m_socketInfo.bev, pData, size) < 0)
		{
			std::cout << "发送数据失败" << std::endl;
		}
	}
	SafeDeleteArray(pListItem->pData);
	SafeDelete(pListItem);
}

void CTCPSocketManage::ThreadSendMsgThread()
{
	std::list <ListItemData*> dataList;
	CDataLine* pDataLine = GetSendDataLine();
	if (!pDataLine)
	{
		std::cout << "send list is null" << std::endl;
		m_running = false;
		return;
	}
	while (m_running)
	{
		if (!pDataLine->SwapDataList(dataList, m_running))
		{
			continue;
		}
		while (!dataList.empty())
		{
			HandleSendData(dataList.front());
			dataList.pop_front();
		}
	}

	std::cout << "send data thread end" << std::endl;

	return;
}