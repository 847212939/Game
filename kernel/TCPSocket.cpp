#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "../Game/stdafx.h"

//接收线程参数
struct RecvThreadParam
{
	CTCPSocketManage* pThis;
	int index;

	RecvThreadParam() :pThis(nullptr), index(0) {}
};

//////////////////////////////////////////////////////////////////////
CTCPSocketManage::CTCPSocketManage()
	:m_listenerBase(NULL),
	m_running(false),
	m_bindIP(""),
	m_socketType(SocketType::SOCKET_TYPE_TCP),
	m_uMaxSocketSize(0),
	m_uCurSocketSize(0),
	m_uCurSocketIndex(0),
	m_pRecvDataLine(new CDataLine),
	m_pSendDataLine(new CDataLine)
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		COUT_LOG(LOG_CERROR, "Init socket dll err");
	}
}

CTCPSocketManage::~CTCPSocketManage()
{
	WSACleanup();
}

bool CTCPSocketManage::Init(int maxCount, int port, const char* ip/* = NULL*/, SocketType socketType /* = SocketType::SOCKET_TYPE_TCP*/)
{
	COUT_LOG(LOG_INFO, "service TCPSocketManage init begin...");

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

	COUT_LOG(LOG_INFO, "service TCPSocketManage init end.");

	return true;
}

bool CTCPSocketManage::UnInit()
{
	// TODO

	return true;
}

bool CTCPSocketManage::Stop()
{
	COUT_LOG(LOG_CINFO, "service TCPSocketManage stop begin...");

	if (!m_running)
	{
		COUT_LOG(LOG_CERROR, "TCPSocketManage is not running......");
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

	COUT_LOG(LOG_INFO, "service TCPSocketManage stop end...");

	return true;
}

bool CTCPSocketManage::Start(ServiceType serverType)
{
	COUT_LOG(LOG_CINFO, "service TCPSocketManage start begin...");

	if (m_running == true)
	{
		COUT_LOG(LOG_CERROR, "service TCPSocketManage already have been running");
		return false;
	}

	m_running = true;
	m_iServiceType = serverType;
	m_uCurSocketSize = 0;
	m_uCurSocketIndex = 0;

	// 创建发送队列
	if (m_pSendDataLine == NULL)
	{
		m_pSendDataLine = new CDataLine;
	}

	// 创建发送线程
	m_socketThread.push_back(new std::thread(ThreadSendMsgThread, this));
	// 创建连接线程
	m_socketThread.push_back(new std::thread(ThreadAcceptThread, this));

	return true;
}

void CTCPSocketManage::ThreadSendMsgThread(void* pThreadData)
{
	CTCPSocketManage* pThis = (CTCPSocketManage*)pThreadData;
	if (!pThis)
	{
		COUT_LOG(LOG_CERROR, "thread param is null");
		return;
	}

	CDataLine* pDataLine = pThis->GetSendDataLine();

	//数据缓存
	void* pDataLineHead = NULL;

	std::this_thread::sleep_for(std::chrono::seconds(3));

	COUT_LOG(LOG_INFO, "CTCPSocketManage::ThreadSendMsgThread thread begin...");

	while (pThis->m_running)
	{
		//获取数据
		unsigned int bytes = pDataLine->GetData(&pDataLineHead);
		if (bytes == 0 || pDataLineHead == NULL)
		{
			continue;
		}

		//处理数据
		SendDataLineHead* pSocketSend = reinterpret_cast<SendDataLineHead*>(pDataLineHead);
		unsigned int size = pSocketSend->dataLineHead.uSize;
		int index = pSocketSend->socketIndex;
		void* pData = static_cast<char*>(pDataLineHead) + sizeof(SendDataLineHead);

		if (index >= 0 && index < pThis->m_socketInfoVec.size())
		{
			TCPSocketInfo& tcpInfo = pThis->m_socketInfoVec[index];
			if (tcpInfo.lock)
			{
				std::lock_guard<std::mutex> guard(*tcpInfo.lock);
				if (tcpInfo.isConnect && tcpInfo.bev)
				{
					if (bufferevent_write(tcpInfo.bev, pData, size) < 0)
					{
						COUT_LOG(LOG_CERROR, "发送数据失败，index=%d socketfd=%d bev=%p,", index, tcpInfo.acceptFd, tcpInfo.bev);
					}
				}
			}
		}
		else
		{
			COUT_LOG(LOG_CERROR, "发送数据失败，index=%d 超出范围", index);
		}

		// 释放内存
		if (pDataLineHead)
		{
			SafeDeleteArray(pDataLineHead);
		}
	}

	COUT_LOG(LOG_INFO, "CTCPSocketManage::ThreadSendMsgThread exit.");

	return;
}

void CTCPSocketManage::ThreadAcceptThread(void* pThreadData)
{
	CTCPSocketManage* pThis = (CTCPSocketManage*)pThreadData;
	if (!pThis)
	{
		COUT_LOG(LOG_CERROR, "thread param is null");
		return;
	}

	std::this_thread::sleep_for(std::chrono::seconds(1));

	COUT_LOG(LOG_INFO, "ThreadAcceptThread thread begin...");

	// libevent服务器 
	struct evconnlistener* listener;
	struct sockaddr_in sin;

	pThis->m_listenerBase = event_base_new();
	if (!pThis->m_listenerBase)
	{
		COUT_LOG(LOG_CERROR, "TCP Could not initialize libevent!");
		return;
	}

	// 开始监听
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(pThis->m_port);
	sin.sin_addr.s_addr = strlen(pThis->m_bindIP) == 0 ? INADDR_ANY : inet_addr(pThis->m_bindIP);

	listener = evconnlistener_new_bind(pThis->m_listenerBase, ListenerCB, (void*)pThis, LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE | LEV_OPT_THREADSAFE, -1, (struct sockaddr*)&sin, sizeof(sin));

	if (!listener)
	{
		COUT_LOG(LOG_INFO, "Could not create a listener! 尝试换个端口或者稍等一会。");
		return;
	}

	evconnlistener_set_error_cb(listener, AcceptErrorCB);

	// 获取接收线程池数量
	/*const CommonConfig& commonConfig = ConfigManage()->GetCommonConfig();
	int workBaseCount = commonConfig.WorkThreadNumber;*/
	int workBaseCount = 0;
	if (workBaseCount <= 1)
	{
		workBaseCount = 4;
	}

	// 初始工作线程信息
	std::shared_ptr<RecvThreadParam[]> uniqueParam(new RecvThreadParam[workBaseCount], [](RecvThreadParam* p) { SafeDeleteArray(p); });
	int socketPairBufSize = sizeof(PlatformSocketInfo) * MAX_POST_CONNECTED_COUNT;
	for (int i = 0; i < workBaseCount; i++)
	{
		uniqueParam[i].index = i;
		uniqueParam[i].pThis = pThis;

		WorkThreadInfo workInfo;
		SOCKET fd[2];
		if (Socketpair(AF_INET, SOCK_STREAM, 0, fd) < 0)
		{
			COUT_LOG(LOG_CERROR, "Socketpair");
			return;
		}

		workInfo.read_fd = fd[0];
		workInfo.write_fd = fd[1];

		SetTcpRcvSndBUF(workInfo.read_fd, socketPairBufSize, socketPairBufSize);
		SetTcpRcvSndBUF(workInfo.write_fd, socketPairBufSize, socketPairBufSize);

		workInfo.base = event_base_new();
		if (!workInfo.base)
		{
			COUT_LOG(LOG_CERROR, "TCP Could not initialize libevent!");
			return;
		}

		workInfo.event = event_new(workInfo.base, workInfo.read_fd, EV_READ, ThreadLibeventProcess, (void*)&uniqueParam[i]);
		if (!workInfo.event)
		{
			COUT_LOG(LOG_CERROR, "TCP Could not create event!");
			return;
		}

		if (event_add(workInfo.event, NULL) < 0)
		{
			COUT_LOG(LOG_CERROR, "TCP event_add ERROR");
			return;
		}

		pThis->m_workBaseVec.push_back(workInfo);
	}

	std::vector<std::thread> threadVev;
	// 开辟工作线程池
	for (int i = 0; i < workBaseCount; i++)
	{
		threadVev.push_back(std::thread(ThreadRSSocket, (void*)&uniqueParam[i]));
	}

	event_base_dispatch(pThis->m_listenerBase);

	evconnlistener_free(listener);
	event_base_free(pThis->m_listenerBase);
	for (int i = 0; i < workBaseCount; i++)
	{
		threadVev[i].join();
		event_base_free(pThis->m_workBaseVec[i].base);
		event_free(pThis->m_workBaseVec[i].event);
	}

	COUT_LOG(LOG_INFO, "ThreadAcceptThread thread exit.");

	return;
}

void CTCPSocketManage::ListenerCB(evconnlistener* listener, evutil_socket_t fd, sockaddr* sa, int socklen, void* data)
{
	CTCPSocketManage* pThis = (CTCPSocketManage*)data;

	// 获取连接信息
	struct sockaddr_in* addrClient = (struct sockaddr_in*)sa;
	PlatformSocketInfo tcpInfo;
	tcpInfo.acceptMsgTime = time(NULL);
	strcpy(tcpInfo.ip, inet_ntoa(addrClient->sin_addr));
	tcpInfo.port = ntohs(addrClient->sin_port);
	tcpInfo.acceptFd = fd;	//服务器accept返回套接字用来和客户端通信

	if (pThis->GetCurSocketSize() >= pThis->m_uMaxSocketSize)
	{
		COUT_LOG(LOG_CERROR, "服务器已经满：fd=%d [ip:%s %d][人数：%u/%u]", fd, tcpInfo.ip, tcpInfo.port, pThis->GetCurSocketSize(), pThis->m_uMaxSocketSize);

		// 分配失败
		NetMessageHead netHead;

		netHead.uMainID = 100;
		netHead.uAssistantID = 3;
		netHead.uHandleCode = ERROR_SERVICE_FULL;//服务器人数已满
		netHead.uMessageSize = sizeof(NetMessageHead);

		sendto(fd, (char*)&netHead, sizeof(NetMessageHead), 0, (sockaddr*)&sa, sizeof(sockaddr_in));

		closesocket(fd);

		return;
	}

	// 设置底层收发缓冲区
	SetTcpRcvSndBUF(fd, SOCKET_RECV_BUF_SIZE, SOCKET_SEND_BUF_SIZE);

	// memcached中线程负载均衡算法
	static int lastThreadIndex = 0;
	lastThreadIndex = lastThreadIndex % pThis->m_workBaseVec.size();

	// 投递到接收线程
	if (send(pThis->m_workBaseVec[lastThreadIndex].write_fd, (const char*)(&tcpInfo), sizeof(tcpInfo), 0) < sizeof(tcpInfo))
	{
		COUT_LOG(LOG_CERROR, "投递连接消息失败,fd=%d", fd);
	}

	lastThreadIndex++;
}

void CTCPSocketManage::ThreadRSSocket(void* pThreadData)
{
	RecvThreadParam* param = (RecvThreadParam*)pThreadData;
	if (!param)
	{
		COUT_LOG(LOG_CERROR, "thread param is null");
		return;
	}

	// 处于监听状态
	event_base_dispatch(param->pThis->m_workBaseVec[param->index].base);

	return;
}

void CTCPSocketManage::ThreadLibeventProcess(evutil_socket_t readfd, short which, void* arg)
{
	RecvThreadParam* param = (RecvThreadParam*)arg;
	CTCPSocketManage* pThis = param->pThis;
	int threadIndex = param->index;
	if (threadIndex < 0 || threadIndex >= pThis->m_workBaseVec.size() || readfd != pThis->m_workBaseVec[threadIndex].read_fd)
	{
		COUT_LOG(LOG_CERROR, "######  threadIndex = %d", threadIndex);
		return;
	}

	char buf[sizeof(PlatformSocketInfo) * MAX_POST_CONNECTED_COUNT] = "";

	int realAllSize = recv(readfd, buf, sizeof(buf), 0);
	if (realAllSize < sizeof(PlatformSocketInfo) || realAllSize % sizeof(PlatformSocketInfo) != 0)
	{
		COUT_LOG(LOG_CERROR, "#### ThreadLibeventProcess error size=%d,sizeof(PlatformSocketInfo)=%lld  ######", realAllSize, sizeof(PlatformSocketInfo));
		event_add(pThis->m_workBaseVec[threadIndex].event, NULL);
		return;
	}

	int countAcceptCount = realAllSize / sizeof(PlatformSocketInfo);
	for (int i = 0; i < countAcceptCount; i++)
	{
		PlatformSocketInfo* pTCPSocketInfo = (PlatformSocketInfo*)(buf + i * sizeof(PlatformSocketInfo));

		// 处理连接
		pThis->AddTCPSocketInfo(threadIndex, pTCPSocketInfo);
	}

	event_add(pThis->m_workBaseVec[threadIndex].event, NULL);
}

void CTCPSocketManage::AddTCPSocketInfo(int threadIndex, PlatformSocketInfo* pTCPSocketInfo)
{
	struct event_base* base = m_workBaseVec[threadIndex].base;
	struct bufferevent* bev = NULL;
	SOCKET fd = pTCPSocketInfo->acceptFd;

	// 分配索引算法
	int index = GetSocketIndex();
	if (index < 0)
	{
		COUT_LOG(LOG_CERROR, "分配索引失败！！！fd=%d,ip=%s", fd, pTCPSocketInfo->ip);
		closesocket(fd);
		return;
	}

	bev = bufferevent_socket_new(base, fd, /*BEV_OPT_CLOSE_ON_FREE |*/ BEV_OPT_THREADSAFE);
	if (!bev)
	{
		COUT_LOG(LOG_CERROR, "Error constructing bufferevent!,fd=%d,ip=%s", fd, pTCPSocketInfo->ip);
		closesocket(fd);
		return;
	}

	// 设置应用层收发数据包，单次大小
	SetMaxSingleReadAndWrite(bev, SOCKET_RECV_BUF_SIZE, SOCKET_SEND_BUF_SIZE);

	// 生成回调函数参数，调用bufferevent_free要释放内存，否则内存泄露
	RecvThreadParam* pRecvThreadParam = new RecvThreadParam;
	pRecvThreadParam->pThis = this;
	pRecvThreadParam->index = index;

	// 添加事件，并设置好回调函数
	bufferevent_setcb(bev, ReadCB, NULL, EventCB, (void*)pRecvThreadParam);
	if (bufferevent_enable(bev, EV_READ | EV_ET) < 0)
	{
		COUT_LOG(LOG_CERROR, "add event fail!!!,fd=%d,ip=%s", fd, pTCPSocketInfo->ip);
		closesocket(fd);
		bufferevent_free(bev);
		SafeDelete(pRecvThreadParam);
		return;
	}

	// 设置读超时，当做心跳。网关服务器才需要
	if (m_iServiceType == ServiceType::SERVICE_TYPE_LOGON)
	{
		timeval tvRead;
		tvRead.tv_sec = CHECK_HEAETBEAT_SECS * KEEP_ACTIVE_HEARTBEAT_COUNT;
		tvRead.tv_usec = 0;
		bufferevent_set_timeouts(bev, &tvRead, NULL);
	}

	// 保存信息
	TCPSocketInfo tcpInfo;
	memcpy(tcpInfo.ip, pTCPSocketInfo->ip, sizeof(tcpInfo.ip));
	tcpInfo.acceptFd = pTCPSocketInfo->acceptFd;
	tcpInfo.acceptMsgTime = pTCPSocketInfo->acceptMsgTime;
	tcpInfo.bev = bev;
	tcpInfo.isConnect = true;
	tcpInfo.port = pTCPSocketInfo->port;
	if (!tcpInfo.lock)
	{
		tcpInfo.lock = new std::mutex;
	}
	tcpInfo.bHandleAccptMsg = false;

	m_ConditionVariable.GetMutex().lock();	//加锁
	if (m_socketInfoVec[index].isConnect)
	{
		m_ConditionVariable.GetMutex().unlock(); //解锁
		COUT_LOG(LOG_CERROR, "分配索引失败,fd=%d,ip=%s", fd, pTCPSocketInfo->ip);
		closesocket(fd);
		bufferevent_free(bev);
		SafeDelete(pRecvThreadParam);
		return;
	}
	m_socketInfoVec[index] = tcpInfo;
	m_heartBeatSocketSet.insert((unsigned int)index);
	m_uCurSocketSize++;
	m_ConditionVariable.GetMutex().unlock(); //解锁

	// 发送连接成功消息
	//SendData(index, NULL, 0, 1, 1, 0, 0, tcpInfo.bev);

	COUT_LOG(LOG_INFO, "TCP connect [ip=%s port=%d index=%d fd=%d bufferevent=%p]", tcpInfo.ip, tcpInfo.port, index, tcpInfo.acceptFd, tcpInfo.bev);
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
	if (bev == NULL)
	{
		COUT_LOG(LOG_CERROR, "RecvData error bev == NULL");
		return false;
	}

	struct evbuffer* input = bufferevent_get_input(bev);

	size_t maxSingleRead = Min_(evbuffer_get_length(input), SOCKET_RECV_BUF_SIZE);

	std::unique_ptr<char[]> uniqueBuf(new char[maxSingleRead]);

	size_t realAllSize = evbuffer_copyout(input, uniqueBuf.get(), maxSingleRead);
	if (realAllSize <= 0)
	{
		return false;
	}

	// 剩余处理数据
	size_t handleRemainSize = realAllSize;

	// 解出包头
	NetMessageHead* pNetHead = (NetMessageHead*)uniqueBuf.get();

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

		void* pData = NULL;
		if (realSize > 0)
		{
			// 没数据就为NULL
			pData = (void*)(uniqueBuf.get() + realAllSize - handleRemainSize + sizeof(NetMessageHead));
		}

		// 派发数据
		DispatchPacket(bev, index, pNetHead, pData, realSize);

		handleRemainSize -= messageSize;

		pNetHead = (NetMessageHead*)(uniqueBuf.get() + realAllSize - handleRemainSize);
	}

	evbuffer_drain(input, realAllSize - handleRemainSize);

	return true;
}

bool CTCPSocketManage::DispatchPacket(void* pBufferevent, int index, NetMessageHead* pHead, void* pData, int size)
{
	if (!pHead)
	{
		return false;
	}

	if (pHead->uMainID == MSG_MAIN_TEST) //心跳包
	{
		return true;
	}

	if (pHead->uMainID == MSG_MAIN_CONECT) //测试连接包
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

	unsigned int addBytes = pDataLine->AddData(uniqueBuf.get(), size + sizeof(SocketReadLine), HD_SOCKET_READ);

	if (addBytes == 0)
	{
		return false;
	}

	return true;
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
		return NULL;
	}

	return m_socketInfoVec[index].ip;
}

const TCPSocketInfo* CTCPSocketManage::GetTCPSocketInfo(int index)
{
	if (index < 0 || index >= m_socketInfoVec.size())
	{
		return NULL;
	}

	return &m_socketInfoVec[index];
}

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
	bufferevent_getcb(tcpInfo.bev, NULL, NULL, NULL, (void**)&pRecvThreadParam);
	if (pRecvThreadParam)
	{
		SafeDelete(pRecvThreadParam);
	}

	//// 服务器主动发起FIN包
	//if (!isClientAutoClose)
	//{
	//	close(tcpInfo.acceptFd);
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

	COUT_LOG(LOG_CERROR, "TCP close [ip=%s port=%d index=%d fd=%d isClientAutoClose:%d acceptTime=%lld]", tcpInfo.ip, tcpInfo.port, index, tcpInfo.acceptFd, isClientAutoClose, tcpInfo.acceptMsgTime);
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

void CTCPSocketManage::AcceptErrorCB(evconnlistener* listener, void* data)
{
	COUT_LOG(LOG_CERROR, "accept error:%s", evutil_socket_error_to_string(EVUTIL_SOCKET_ERROR()));
}

int CTCPSocketManage::StreamSocketpair(struct addrinfo* addr_info, SOCKET sock[2])
{
	if (!addr_info)
	{
		return -1;
	}

	SOCKET listener, client, server;
	int opt = 1;

	listener = server = client = INVALID_SOCKET;
	listener = socket(addr_info->ai_family, addr_info->ai_socktype, addr_info->ai_protocol); //创建服务器socket并进行绑定监听等
	if (INVALID_SOCKET == listener)
	{
		goto fail;
	}

	setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));

	if (SOCKET_ERROR == bind(listener, addr_info->ai_addr, static_cast<int>(addr_info->ai_addrlen)))
	{
		goto fail;
	}
	if (SOCKET_ERROR == getsockname(listener, addr_info->ai_addr, (int*)&addr_info->ai_addrlen))
	{
		goto fail;
	}
	if (SOCKET_ERROR == listen(listener, 5))
	{
		goto fail;
	}

	client = socket(addr_info->ai_family, addr_info->ai_socktype, addr_info->ai_protocol); //创建客户端socket，并连接服务器

	if (INVALID_SOCKET == client)
	{
		goto fail;
	}
	if (SOCKET_ERROR == connect(client, addr_info->ai_addr, static_cast<int>(addr_info->ai_addrlen)))
	{
		goto fail;
	}

	server = accept(listener, 0, 0);

	if (INVALID_SOCKET == server)
	{
		goto fail;
	}

	closesocket(listener);

	sock[0] = client;
	sock[1] = server;

	return 0;
fail:
	if (INVALID_SOCKET != listener)
	{
		closesocket(listener);
	}
	if (INVALID_SOCKET != client)
	{
		closesocket(client);
	}
	return -1;
}

int CTCPSocketManage::DgramSocketpair(struct addrinfo* addr_info, SOCKET sock[2])
{
	if (!addr_info)
	{
		return -1;
	}
	SOCKET client, server;
	struct addrinfo addr, * result = NULL;
	const char* address;
	int opt = 1;

	server = client = INVALID_SOCKET;

	server = socket(addr_info->ai_family, addr_info->ai_socktype, addr_info->ai_protocol);
	if (INVALID_SOCKET == server)
	{
		goto fail;
	}

	setsockopt(server, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));

	if (SOCKET_ERROR == bind(server, addr_info->ai_addr, static_cast<int>(addr_info->ai_addrlen)))
	{
		goto fail;
	}
	if (SOCKET_ERROR == getsockname(server, addr_info->ai_addr, (int*)&addr_info->ai_addrlen))
	{
		goto fail;
	}

	client = socket(addr_info->ai_family, addr_info->ai_socktype, addr_info->ai_protocol);

	if (INVALID_SOCKET == client)
	{
		goto fail;
	}

	memset(&addr, 0, sizeof(addr));
	addr.ai_family = addr_info->ai_family;
	addr.ai_socktype = addr_info->ai_socktype;
	addr.ai_protocol = addr_info->ai_protocol;

	if (AF_INET6 == addr.ai_family)
	{
		address = "0:0:0:0:0:0:0:1";
	}
	else
	{
		address = "127.0.0.1";
	}
	if (getaddrinfo(address, "0", &addr, &result))
	{
		goto fail;
	}

	setsockopt(client, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));

	if (SOCKET_ERROR == bind(client, result->ai_addr, static_cast<int>(result->ai_addrlen)))
	{
		goto fail;
	}
	if (SOCKET_ERROR == getsockname(client, result->ai_addr, (int*)&result->ai_addrlen))
	{
		goto fail;
	}
	if (SOCKET_ERROR == connect(server, result->ai_addr, static_cast<int>(result->ai_addrlen)))
	{
		goto fail;
	}
	if (SOCKET_ERROR == connect(client, addr_info->ai_addr, static_cast<int>(addr_info->ai_addrlen)))
	{
		goto fail;
	}

	freeaddrinfo(result);
	sock[0] = client;
	sock[1] = server;
	return 0;

fail:
	if (INVALID_SOCKET != client)
	{
		closesocket(client);
	}
	if (INVALID_SOCKET != server)
	{
		closesocket(server);
	}
	if (result)
	{
		freeaddrinfo(result);
	}

	return -1;
}

int CTCPSocketManage::Socketpair(int family, int type, int protocol, SOCKET recv[2])
{
	const char* address;
	struct addrinfo addr_info, * p_addrinfo;
	int result = -1;

	memset(&addr_info, 0, sizeof(addr_info));
	addr_info.ai_family = family;
	addr_info.ai_socktype = type;
	addr_info.ai_protocol = protocol;
	if (AF_INET6 == family)
	{
		address = "0:0:0:0:0:0:0:1";
	}
	else
	{
		address = "127.0.0.1";
	}

	if (0 == getaddrinfo(address, "0", &addr_info, &p_addrinfo))
	{
		if (SOCK_STREAM == type)
		{
			result = StreamSocketpair(p_addrinfo, recv);   //use for tcp
		}
		else if (SOCK_DGRAM == type)
		{
			result = DgramSocketpair(p_addrinfo, recv);    //use for udp
		}
		freeaddrinfo(p_addrinfo);
	}
	return result;
}

bool CTCPSocketManage::SendData(int index, void* pData, int size, int mainID, int assistID, int handleCode, void* pBufferevent, unsigned int uIdentification/* = 0*/)
{
	if (!IsConnected(index))
	{
		COUT_LOG(LOG_CERROR, "socketIdx close, index=%d, mainID=%d assistID=%d", index, mainID, assistID);
		return false;
	}

	if (size < 0 || size > MAX_TEMP_SENDBUF_SIZE - sizeof(NetMessageHead))
	{
		COUT_LOG(LOG_CERROR, "invalid message size size=%d", size);
		return false;
	}

	// 整合一下数据
	std::unique_ptr<char[]> unqueBuf(new char[sizeof(SendDataLineHead) + sizeof(NetMessageHead) + size]);

	// 拼接包头
	NetMessageHead* pHead = reinterpret_cast<NetMessageHead*>((char*)unqueBuf.get() + sizeof(SendDataLineHead));
	pHead->uMainID = mainID;
	pHead->uAssistantID = assistID;
	pHead->uMessageSize = sizeof(NetMessageHead) + size;
	pHead->uHandleCode = handleCode;
	pHead->uIdentification = uIdentification;

	// 包体
	if (pData && size > 0)
	{
		memcpy(unqueBuf.get() + sizeof(SendDataLineHead) + sizeof(NetMessageHead), pData, size);
	}

	// 投递到发送队列
	if (m_pSendDataLine)
	{
		SendDataLineHead* pLineHead = reinterpret_cast<SendDataLineHead*>(unqueBuf.get());
		pLineHead->socketIndex = index;
		pLineHead->pBufferevent = pBufferevent;

		unsigned int addBytes = m_pSendDataLine->AddData(pLineHead, sizeof(SendDataLineHead) +  pHead->uMessageSize, 0);

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

bool CTCPSocketManage::GetRuninged()
{
	return m_running;
}

ServiceType CTCPSocketManage::GetServerType()
{
	return m_iServiceType;
}