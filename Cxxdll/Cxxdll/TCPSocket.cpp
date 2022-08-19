#include "pch.h"

CTCPSocketManage::CTCPSocketManage() :
	m_bindIP(""),
	m_running(false),
	m_uMaxSocketSize(0),
	m_uCurSocketSize(0),
	m_uCurSocketIndex(0),
	m_listenerBase(nullptr),
	m_pRecvDataLine(new CDataLine),
	m_pSendDataLine(new CDataLine),
	m_eventBaseCfg(event_config_new()),
	m_socketType(SocketType::SOCKET_TYPE_TCP)
{
	WSADATA wsa;
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		COUT_LOG(LOG_CERROR, "Init socket dll err");
	}
	if (event_config_set_flag(m_eventBaseCfg, EVENT_BASE_FLAG_STARTUP_IOCP))
	{
		COUT_LOG(LOG_CERROR, "Init iocp is err");
	}
	if (evthread_use_windows_threads() != 0)
	{
		COUT_LOG(LOG_CERROR, "Init iocp thread is err");
	}
	if (event_config_set_num_cpus_hint(m_eventBaseCfg, si.dwNumberOfProcessors) != 0)
	{
		COUT_LOG(LOG_CERROR, "Set the number of CPU is err");
	}
}

CTCPSocketManage::~CTCPSocketManage()
{
	WSACleanup();
}

bool CTCPSocketManage::Init(int maxCount, int port, const char* ip, SocketType socketType)
{
	if (maxCount <= 0 || port <= 1000)
	{
		COUT_LOG(LOG_CERROR, "invalid params input maxCount=%d port=%d", maxCount, port);
		return false;
	}
	m_uMaxSocketSize = maxCount;
	if (ip && strlen(ip) < sizeof(m_bindIP))
	{
		strcpy(m_bindIP, ip);
	}

	m_port = port;
	m_socketType = socketType;

	m_workBaseVec.clear();
	m_heartBeatSocketSet.clear();

	// 初始化分配内存
	unsigned int socketInfoVecSize = m_uMaxSocketSize * 2;
	m_socketInfoVec.resize((size_t)socketInfoVecSize);

	return true;
}

bool CTCPSocketManage::Stop()
{
	COUT_LOG(LOG_CINFO, "service tcp stop begin");

	if (!m_running)
	{
		COUT_LOG(LOG_CERROR, "TCPSocketManage is not running");
		return false;
	}

	m_running = false;
	m_uCurSocketSize = 0;
	m_uCurSocketIndex = 0;

	event_base_loopbreak(m_listenerBase);
	for (size_t i = 0; i < m_workBaseVec.size(); i++)
	{
		event_base_loopbreak(m_workBaseVec[i].base);
	}

	for (size_t i = 0; i < m_socketInfoVec.size(); i++)
	{
		if (m_socketInfoVec[i].lock)
		{
			SafeDelete(m_socketInfoVec[i].lock);
		}
	}

	COUT_LOG(LOG_INFO, "service tcp stop end");

	return true;
}

bool CTCPSocketManage::Start(ServiceType serverType)
{
	if (m_running == true)
	{
		COUT_LOG(LOG_CERROR, "service tcp already have been running");
		return false;
	}

	m_running = true;
	m_uCurSocketSize = 0;
	m_uCurSocketIndex = 0;
	m_iServiceType = serverType;

	m_socketThread.push_back(new std::thread(&CTCPSocketManage::ThreadSendMsgThread, this));
	m_socketThread.push_back(new std::thread(&CTCPSocketManage::ConnectServerThread, this));

	return true;
}

void CTCPSocketManage::ConnectServerThread()
{
	m_ConnectServerBase = event_base_new_with_config(m_eventBaseCfg);
	event_config_free(m_eventBaseCfg);

	if (!m_ConnectServerBase)
	{
		COUT_LOG(LOG_CERROR, "TCP Could not initialize libevent!");
		return;
	}

	struct bufferevent* bev = nullptr;

	// 分配索引算法
	int index = GetSocketIndex();
	if (index < 0)
	{
		COUT_LOG(LOG_CERROR, "分配索引失败");
		return;
	}

	bev = bufferevent_socket_new(m_ConnectServerBase, -1, BEV_OPT_CLOSE_ON_FREE | BEV_OPT_THREADSAFE);
	if (!bev)
	{
		COUT_LOG(LOG_CERROR, "Error constructing bufferevent!");
		return;
	}

	// 设置应用层收发数据包，单次大小
	SetMaxSingleReadAndWrite(bev, SOCKET_RECV_BUF_SIZE, SOCKET_SEND_BUF_SIZE);

	// 生成回调函数参数，调用bufferevent_free要释放内存，否则内存泄露
	RecvThreadParam* pRecvThreadParam = new RecvThreadParam;
	pRecvThreadParam->pThis = this;
	pRecvThreadParam->index = index;

	// 添加事件，并设置好回调函数
	bufferevent_setcb(bev, ReadCB, nullptr, EventCB, (void*)pRecvThreadParam);
	if (bufferevent_enable(bev, EV_READ | EV_ET) < 0)
	{
		COUT_LOG(LOG_CERROR, "add event fail!!!");
		bufferevent_free(bev);
		SafeDelete(pRecvThreadParam);
		return;
	}

	// 设置读超时，当做心跳。网关服务器才需要
	if (m_iServiceType == ServiceType::SERVICE_TYPE_LOGIC)
	{
		timeval tvRead;
		tvRead.tv_sec = CHECK_HEAETBEAT_SECS * KEEP_ACTIVE_HEARTBEAT_COUNT;
		tvRead.tv_usec = 0;
		bufferevent_set_timeouts(bev, &tvRead, nullptr);
	}

	const CLogicCfg& logicCfg = CfgMgr->GetCBaseCfgMgr().GetLogicCfg();

	//struct sockaddr_in6 sin6; ipv6
	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(logicCfg.port);
	evutil_inet_pton(sin.sin_family, logicCfg.ip.c_str(), (void*)&sin.sin_addr);

	if (0 != bufferevent_socket_connect(bev, (struct sockaddr*)&sin, sizeof(sin)))
	{
		COUT_LOG(LOG_CERROR, "Could not bufferevent_socket_connect!");
		bufferevent_free(bev);
		SafeDelete(pRecvThreadParam);
		return;
	}

	COUT_LOG(LOG_CINFO, "初始化完成");

	event_base_dispatch(m_listenerBase);

	event_base_free(m_listenerBase);
}

void CTCPSocketManage::ReadCB(bufferevent* bev, void* data)
{
	RecvThreadParam* param = (RecvThreadParam*)data;
	CTCPSocketManage* pThis = param->pThis;
	int index = param->index;

	// 处理数据，包头解析
	pThis->RecvData(bev, index);
}

bool CTCPSocketManage::RecvData(bufferevent* bev, int index)
{
	if (bev == nullptr)
	{
		COUT_LOG(LOG_CERROR, "RecvData error bev == nullptr");
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
		CloseSocket(index);
		COUT_LOG(LOG_CERROR, "消息格式不正确,index=%d", index);
		return false;
	}

	// 粘包处理
	while (handleRemainSize >= sizeof(NetMessageHead) && handleRemainSize >= pNetHead->uMessageSize)
	{
		unsigned int messageSize = pNetHead->uMessageSize;
		if (messageSize > MAX_TEMP_SENDBUF_SIZE)
		{
			// 消息格式不正确
			CloseSocket(index);
			COUT_LOG(LOG_CERROR, "消息格式不正确");
			return false;
		}

		int realSize = messageSize - sizeof(NetMessageHead);
		if (realSize < 0)
		{
			// 数据包不够包头
			CloseSocket(index);
			COUT_LOG(LOG_CERROR, "数据包不够包头");
			return false;
		}

		void* pData = nullptr;
		if (realSize > 0)
		{
			// 没数据就为nullptr
			pData = (void*)(recvBuf.get() + realAllSize - handleRemainSize + sizeof(NetMessageHead));
		}

		// 派发数据
		DispatchPacket(bev, index, pNetHead, pData, realSize);

		handleRemainSize -= messageSize;

		pNetHead = (NetMessageHead*)(recvBuf.get() + realAllSize - handleRemainSize);
	}

	evbuffer_drain(input, realAllSize - handleRemainSize);

	return true;
}

bool CTCPSocketManage::DispatchPacket(void* pBufferevent, int index, NetMessageHead* pHead, void* pData, int size)
{
	if (!pBufferevent || !pHead)
	{
		return false;
	}
	if (pHead->uMainID == (unsigned int)MsgCmd::MsgCmd_HeartBeat) //心跳包
	{
		return true;
	}
	if (pHead->uMainID == (unsigned int)MsgCmd::MsgCmd_Testlink) //测试连接包
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
	msg.uIndex = index;
	msg.pBufferevent = pBufferevent;
	msg.uAccessIP = 0;
	msg.netMessageHead = *pHead;
	msg.socketType = m_socketType;

	std::unique_ptr<char[]> uniqueBuf(new char[size + sizeof(SocketReadLine)]);
	memcpy(uniqueBuf.get(), &msg, sizeof(SocketReadLine));
	memcpy(uniqueBuf.get() + sizeof(SocketReadLine), pData, size);

	unsigned int addBytes = pDataLine->AddData(uniqueBuf.get(), size + sizeof(SocketReadLine), SysMsgCmd::HD_SOCKET_READ);

	if (addBytes == 0)
	{
		return false;
	}

	return true;
}

//网络关闭处理
bool CTCPSocketManage::OnSocketCloseEvent(ULONG uAccessIP, UINT uIndex, UINT uConnectTime, BYTE socketType)
{
	SocketCloseLine SocketClose;
	SocketClose.uConnectTime = uConnectTime;
	SocketClose.uIndex = uIndex;
	SocketClose.uAccessIP = uAccessIP;
	SocketClose.socketType = socketType;
	return (m_pRecvDataLine->AddData(&SocketClose, sizeof(SocketClose), SysMsgCmd::HD_SOCKET_CLOSE) != 0);
}

bool CTCPSocketManage::CloseSocket(int index)
{
	RemoveTCPSocketStatus(index);

	return true;
}

CDataLine* CTCPSocketManage::GetRecvDataLine()
{
	return m_pRecvDataLine;
}

CDataLine* CTCPSocketManage::GetSendDataLine()
{
	return m_pSendDataLine;
}

unsigned int CTCPSocketManage::GetCurSocketSize()
{
	return m_uCurSocketSize;
}

bool CTCPSocketManage::IsConnected(int index)
{
	if (index < 0 || index >= m_socketInfoVec.size())
	{
		return false;
	}

	return m_socketInfoVec[index].isConnect;
}

void CTCPSocketManage::GetSocketSet(std::vector<UINT>& vec)
{
	vec.clear();

	std::lock_guard<std::mutex> guard(m_ConditionVariable.GetMutex());

	for (auto iter = m_heartBeatSocketSet.begin(); iter != m_heartBeatSocketSet.end(); iter++)
	{
		vec.push_back(*iter);
	}
}

const std::vector<TCPSocketInfo>& CTCPSocketManage::GetSocketVector()
{
	return m_socketInfoVec;
}

const char* CTCPSocketManage::GetSocketIP(int index)
{
	if (index < 0 || index >= m_socketInfoVec.size())
	{
		return nullptr;
	}

	return m_socketInfoVec[index].ip;
}

const TCPSocketInfo* CTCPSocketManage::GetTCPSocketInfo(int index)
{
	if (index < 0 || index >= m_socketInfoVec.size())
	{
		return nullptr;
	}

	return &m_socketInfoVec[index];
}

// 分配索引算法
int CTCPSocketManage::GetSocketIndex()
{
	std::lock_guard<std::mutex> guard(m_ConditionVariable.GetMutex());

	m_uCurSocketIndex = m_uCurSocketIndex % m_socketInfoVec.size();
	int index = -1;

	for (unsigned int iCount = 0; iCount < m_uMaxSocketSize * 2; iCount++)
	{
		if (!m_socketInfoVec[m_uCurSocketIndex].isConnect)
		{
			index = m_uCurSocketIndex;
			m_uCurSocketIndex++;
			break;
		}
	}

	if (index >= m_socketInfoVec.size())
	{
		return -1;
	}

	return index;
}

void CTCPSocketManage::RemoveTCPSocketStatus(int index, bool isClientAutoClose/* = false*/)
{
	if (index < 0 || index >= m_socketInfoVec.size())
	{
		COUT_LOG(LOG_CERROR, "index=%d 超出范围", index);
		return;
	}

	ULONG uAccessIP = 0;

	// 加锁
	m_ConditionVariable.GetMutex().lock();

	auto& tcpInfo = m_socketInfoVec[index];

	// 重复调用
	if (!tcpInfo.isConnect)
	{
		return;
	}

	// 如果锁没有分配内存，就分配
	if (!tcpInfo.lock)
	{
		tcpInfo.lock = new std::mutex;
	}

	uAccessIP = inet_addr(tcpInfo.ip);
	m_uCurSocketSize--;
	m_heartBeatSocketSet.erase((UINT)index);

	// 释放参数内存
	RecvThreadParam* pRecvThreadParam = (RecvThreadParam*)0x01;
	bufferevent_getcb(tcpInfo.bev, nullptr, nullptr, nullptr, (void**)&pRecvThreadParam);
	if (pRecvThreadParam)
	{
		SafeDelete(pRecvThreadParam);
	}

	//// 服务器主动发起FIN包
	//if (!isClientAutoClose)
	//{
	//	closesocket(tcpInfo.acceptFd);
	//}

	// 和发送线程相关的锁
	tcpInfo.lock->lock();

	tcpInfo.isConnect = false;
	bufferevent_free(tcpInfo.bev);
	tcpInfo.bev = nullptr;

	// 解锁发送线程
	tcpInfo.lock->unlock();

	// 解锁多线程
	m_ConditionVariable.GetMutex().unlock();

	// 如果没有设置BEV_OPT_CLOSE_ON_FREE 选项，则关闭socket
	closesocket(tcpInfo.acceptFd);

	OnSocketCloseEvent(uAccessIP, index, (UINT)tcpInfo.acceptMsgTime, (BYTE)m_socketType);

	COUT_LOG(LOG_CINFO, "TCP close [ip=%s port=%d index=%d fd=%d isClientAutoClose:%d acceptTime=%lld]",
		tcpInfo.ip, tcpInfo.port, index, tcpInfo.acceptFd, isClientAutoClose, tcpInfo.acceptMsgTime);
}

void CTCPSocketManage::SetTcpRcvSndBUF(SOCKET fd, int rcvBufSize, int sndBufSize)
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
		COUT_LOG(LOG_CERROR, "bufferevent_set_max_single_read fail,bev=%p", bev);
	}

	/*if (bufferevent_set_max_single_write(bev, sndBufSize) < 0)
	{
		COUT_LOG(LOG_CERROR, "bufferevent_set_max_single_write fail,fd=%d", fd);
	}*/
}

void CTCPSocketManage::EventCB(bufferevent* bev, short events, void* data)
{
	RecvThreadParam* param = (RecvThreadParam*)data;
	CTCPSocketManage* pThis = param->pThis;
	int index = param->index;

	if (events & BEV_EVENT_EOF)
	{
		// 正常结束
	}
	else if (events & BEV_EVENT_ERROR)
	{
		// windows正常结束
	}
	else if (events & BEV_EVENT_TIMEOUT) // 长时间没有收到，客户端发过来的数据，读取数据超时
	{
		COUT_LOG(LOG_INFO, "心跳踢人 index=%d fd=%d", index, pThis->m_socketInfoVec[index].acceptFd);
	}
	else
	{
		COUT_LOG(LOG_CERROR, "Got an error on the connection,events=%d", events);
	}

	pThis->RemoveTCPSocketStatus(index, true);
}

bool CTCPSocketManage::SendData(int index, const char* pData, size_t size, MsgCmd mainID, int assistID, int handleCode, void* pBufferevent, unsigned int uIdentification/* = 0*/)
{
	if (!pBufferevent)
	{
		COUT_LOG(LOG_CERROR, "!pBufferevent");
		return false;
	}
	if (!IsConnected(index))
	{
		COUT_LOG(LOG_CERROR, "socketIdx close, index=%d, mainID=%d assistID=%d", index, mainID, assistID);
		return false;
	}

	if (size < 0 || size > MAX_TEMP_SENDBUF_SIZE - sizeof(NetMessageHead))
	{
		COUT_LOG(LOG_CERROR, "invalid message size size=%lld", size);
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
		pLineHead->socketIndex = index;
		pLineHead->pBufferevent = pBufferevent;

		unsigned int addBytes = m_pSendDataLine->AddData(pLineHead, sizeof(SendDataLineHead) + pHead->uMessageSize);

		if (addBytes == 0)
		{
			COUT_LOG(LOG_CERROR, "投递消息失败,mainID=%d,assistID=%d", mainID, assistID);
			return false;
		}
	}

	return true;
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
	int index = pSocketSend->socketIndex;
	void* pData = pListItem->pData + sizeof(SendDataLineHead);

	if (index < 0 || index >= m_socketInfoVec.size())
	{
		COUT_LOG(LOG_CERROR, "发送数据失败，index=%d 超出范围", index);
		return;
	}
	TCPSocketInfo& tcpInfo = m_socketInfoVec[index];
	if (!tcpInfo.lock)
	{
		return;
	}
	{
		std::lock_guard<std::mutex> guard(*tcpInfo.lock);
		if (!tcpInfo.isConnect || !tcpInfo.bev)
		{
			return;
		}
		if (bufferevent_write(tcpInfo.bev, pData, size) < 0)
		{
			COUT_LOG(LOG_CERROR, "发送数据失败，index=%d socketfd=%d bev=%p,", index, tcpInfo.acceptFd, tcpInfo.bev);
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
		COUT_LOG(LOG_CERROR, "send list is null");
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

	COUT_LOG(LOG_CINFO, "send data thread end");

	return;
}