#include "../stdafx.h"

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
	m_ctx(nullptr),
	m_DBServerIndex(-1),
	m_CrossServerIndex(-1)
{
#if defined(_WIN32)
	WSADATA wsa;
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		Log(CERR, "Init socket dll err");
	}
	if (event_config_set_flag(m_eventBaseCfg, EVENT_BASE_FLAG_STARTUP_IOCP))
	{
		Log(CERR, "Init iocp is err");
	}
	if (evthread_use_windows_threads() != 0)
	{
		Log(CERR, "Init iocp thread is err");
	}
	if (event_config_set_num_cpus_hint(m_eventBaseCfg, si.dwNumberOfProcessors) != 0)
	{
		Log(CERR, "Set the number of CPU is err");
	}
#elif defined(_WIN64)
#elif defined(__linux__)
	if (evthread_use_pthreads() != 0)
	{
		Log(CERR, "Init thread is err");
	}
#elif defined(__unix__)
#elif defined(__ANDROID__)
#elif defined(__APPLE__)
#endif
}
CTCPSocketManage::~CTCPSocketManage()
{
#if defined(_WIN32)
	WSACleanup();
#elif defined(_WIN64)
#elif defined(__linux__)
#elif defined(__unix__)
#elif defined(__ANDROID__)
#elif defined(__APPLE__)
#endif
}

bool CTCPSocketManage::Init(int maxCount, int port, const char* ip, 
	ServiceType serverType/* = ServiceType::SERVICE_TYPE_BEGIN*/)
{
	if (maxCount <= 0 || port <= 1000)
	{
		Log(CERR, "invalid params input maxCount=%d port=%d", maxCount, port);
		return false;
	}
	m_uMaxSocketSize = maxCount;
	if (ip && strlen(ip) < sizeof(m_bindIP))
	{
		strcpy(m_bindIP, ip);
	}

	m_port = port;
	m_ServiceType = serverType;
	m_workBaseVec.clear();
	m_heartBeatSocketSet.clear();

	// 初始化分配内存
	unsigned int socketInfoVecSize = m_uMaxSocketSize * 2;
	m_socketInfoVec.resize((size_t)socketInfoVecSize);

	if (m_ServiceType == ServiceType::SERVICE_TYPE_LOGIC_WSS)
	{
#ifdef __WebSocketOpenssl__
		if (!OpensslInit())
		{
			return false;
		}
#endif
	}

	return true;
}
bool CTCPSocketManage::WaitConnect(int threadIndex)
{
	// 获取接收线程池数量
	if (threadIndex >= G_CfgMgr->GetCBaseCfgMgr().GetThreadCnt())
	{
		return false;
	}
	if (m_workBaseVec.size() <= threadIndex)
	{
		while (true)
		{
			std::this_thread::sleep_for(std::chrono::microseconds(10));
			if (m_workBaseVec.size() > threadIndex)
			{
				struct event_base* base = m_workBaseVec[threadIndex].base;
				if (base)
				{
					break;
				}
			}
		}
	}
	return true;
}
bool CTCPSocketManage::IsServerMsg(int index)
{
	if (index == GetDBServerIndex())
	{
		return true;
	}
	if (m_ServiceType == ServiceType::SERVICE_TYPE_CROSS)
	{
		return true;;
	}
	else
	{
		if (index == GetCrossServerIndex())
		{
			return true;
		}
	}
	
	return false;
}
SOCKFD CTCPSocketManage::GetNewSocket()
{
	SOCKFD sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
	{
		Log(CINF, "申请socket失败");
		return false;
	}

	return sock;
}
bool CTCPSocketManage::ConnectCrossServer(SOCKFD& sock, int threadIndex)
{
	const CLogicCfg& serverCfg = G_CfgMgr->GetCBaseCfgMgr().GetCrossServerCfg();
	
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(serverCfg.port);
	sin.sin_addr.s_addr = inet_addr(serverCfg.ip.c_str());

	if (connect(sock, (sockaddr*)&sin, sizeof(sockaddr_in)) < 0)
	{
		Log(CINF, "连接服跨服失败");
		return false;
	}

	// 获取连接信息
	PlatformSocketInfo tcpInfo;
	tcpInfo.acceptMsgTime = time(nullptr);
	memcpy(tcpInfo.ip, serverCfg.ip.c_str(), serverCfg.ip.size());
	tcpInfo.port = serverCfg.port;
	tcpInfo.acceptFd = sock;	//服务器accept返回套接字用来和客户端通信

	m_CrossServerIndex = AddServerSocketInfo(threadIndex, &tcpInfo);

	Log(CINF, "连接服跨服成功 [ip=%s port=%d index=%d fd=%d]",
		tcpInfo.ip, tcpInfo.port, m_CrossServerIndex, tcpInfo.acceptFd);
	return true;
}
bool CTCPSocketManage::ConnectDBServer(SOCKFD& sock, int threadIndex)
{
	const CLogicCfg& serverCfg = G_CfgMgr->GetCBaseCfgMgr().GetDBServerCfg();

	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(serverCfg.port);
	sin.sin_addr.s_addr = inet_addr(serverCfg.ip.c_str());

	if (connect(sock, (sockaddr*)&sin, sizeof(sockaddr_in)) < 0)
	{
		Log(CINF, "连接服DB失败");
		return false;
	}

	// 获取连接信息
	PlatformSocketInfo tcpInfo;
	tcpInfo.acceptMsgTime = time(nullptr);
	memcpy(tcpInfo.ip, serverCfg.ip.c_str(), serverCfg.ip.size());
	tcpInfo.port = serverCfg.port;
	tcpInfo.acceptFd = sock;	//服务器accept返回套接字用来和客户端通信

	m_DBServerIndex = AddServerSocketInfo(threadIndex, &tcpInfo);

	Log(CINF, "连接服DB成功 [ip=%s port=%d index=%d fd=%d]",
		tcpInfo.ip, tcpInfo.port, m_DBServerIndex, tcpInfo.acceptFd);
	return true;
}
void CTCPSocketManage::Sleepseconds(int seconds)
{
	std::this_thread::sleep_for(std::chrono::seconds(seconds));
}
bool CTCPSocketManage::ConnectServer()
{
	SOCKFD sock = 0;
	for (int i = 0; i < 2; i++)
	{
		if (!WaitConnect(i))
		{
			return false;
		}
	}
	if (m_ServiceType == ServiceType::SERVICE_TYPE_CROSS)
	{
		sock = GetNewSocket();
		while (true)
		{
			if (ConnectDBServer(sock, 0))
			{
				break;
			}
			Sleepseconds(5);
		}
	}
	else
	{
		sock = GetNewSocket();
		while (true)
		{
			if (ConnectDBServer(sock, 0))
			{
				break;
			}
			Sleepseconds(5);
		}

		sock = GetNewSocket();
		while (true)
		{
			if (ConnectCrossServer(sock, 1))
			{
				break;
			}
			Sleepseconds(5);
		}
	}

	return true;
}
bool CTCPSocketManage::Stop()
{
	Log(CINF, "service tcp stop begin");

	if (!m_running)
	{
		Log(CERR, "TCPSocketManage is not running");
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

	Log(INF, "service tcp stop end");

	return true;
}
bool CTCPSocketManage::Start()
{
	if (m_running == true)
	{
		Log(CERR, "service tcp already have been running");
		return false;
	}

	m_running = true;
	m_uCurSocketSize = 0;
	m_uCurSocketIndex = 0;

	m_socketThread.push_back(new std::thread(&CTCPSocketManage::ThreadSendMsg, this));
	m_socketThread.push_back(new std::thread(&CTCPSocketManage::ThreadAccept, this));

	return true;
}

void CTCPSocketManage::ThreadAccept()
{
	struct sockaddr_in sin;
	struct evconnlistener* listener;

	//m_listenerBase = event_base_new();
	m_listenerBase = event_base_new_with_config(m_eventBaseCfg);
	event_config_free(m_eventBaseCfg);

	if (!m_listenerBase)
	{
		Log(CERR, "TCP Could not initialize libevent!");
		return;
	}

	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(m_port);
	sin.sin_addr.s_addr = strlen(m_bindIP) == 0 ? INADDR_ANY : inet_addr(m_bindIP);

	listener = evconnlistener_new_bind(m_listenerBase, ListenerCB, (void*)this,
		LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE | LEV_OPT_THREADSAFE,
		-1, (struct sockaddr*)&sin, sizeof(sin));

	if (!listener)
	{
		Log(INF, "Could not create a listener! 尝试换个端口或者稍等一会。");
		return;
	}

	evconnlistener_set_error_cb(listener, AcceptErrorCB);

	// 获取接收线程池数量
	int workBaseCount = G_CfgMgr->GetCBaseCfgMgr().GetThreadCnt();
	if (workBaseCount <= 1)
	{
		workBaseCount = 8;
	}

	// 初始工作线程信息
	std::shared_ptr<RecvThreadParam[]> uniqueParam(new RecvThreadParam[workBaseCount],
		[](RecvThreadParam* p)
		{
			SafeDeleteArray(p);
		});
	int socketPairBufSize = sizeof(PlatformSocketInfo) * MAX_POST_CONNECTED_COUNT;
	for (int i = 0; i < workBaseCount; i++)
	{
		uniqueParam[i].index = i;
		uniqueParam[i].pThis = this;

		WorkThreadInfo workInfo;
		SOCKFD fd[2];
		if (Socketpair(AF_INET, SOCK_STREAM, 0, fd) < 0)
		{
			Log(CERR, "Socketpair");
			return;
		}

		workInfo.read_fd = fd[0];
		workInfo.write_fd = fd[1];

		SetTcpRcvSndBUF(workInfo.read_fd, socketPairBufSize, socketPairBufSize);
		SetTcpRcvSndBUF(workInfo.write_fd, socketPairBufSize, socketPairBufSize);

		workInfo.base = event_base_new();
		//workInfo.base = event_base_new_with_config(m_eventBaseCfg);
		if (!workInfo.base)
		{
			Log(CERR, "TCP Could not initialize libevent!");
			return;
		}

		workInfo.event = event_new(workInfo.base, workInfo.read_fd, EV_READ,
			ThreadLibeventProcess, (void*)&uniqueParam[i]);
		if (!workInfo.event)
		{
			Log(CERR, "TCP Could not create event!");
			return;
		}

		if (event_add(workInfo.event, nullptr) < 0)
		{
			Log(CERR, "TCP event_add ERROR");
			return;
		}

		m_workBaseVec.push_back(workInfo);
	}

	std::vector<std::thread> threadVev;
	// 开辟工作线程池
	for (int i = 0; i < workBaseCount; i++)
	{
		threadVev.push_back(std::thread(ThreadRSSocket, (void*)&uniqueParam[i]));
	}

	event_base_dispatch(m_listenerBase);

	evconnlistener_free(listener);
	event_base_free(m_listenerBase);
	//event_config_free(m_eventBaseCfg);

	for (int i = 0; i < workBaseCount; i++)
	{
		threadVev[i].join();

		WorkThreadInfo& workInfo = m_workBaseVec[i];

		closesocket(workInfo.read_fd);
		closesocket(workInfo.write_fd);

		if (workInfo.base)
		{
			event_base_free(workInfo.base);
		}
		if (workInfo.event)
		{
			// 不知道为什么退出时发生崩溃
			//event_free(workInfo.event);
		}
	}

	Log(CINF, "accept thread end");

	return;
}
void CTCPSocketManage::ThreadRSSocket(void* pThreadData)
{
	RecvThreadParam* param = (RecvThreadParam*)pThreadData;
	if (!param)
	{
		Log(CERR, "thread param is null");
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
	if (threadIndex < 0 || threadIndex >= pThis->m_workBaseVec.size()
		|| readfd != pThis->m_workBaseVec[threadIndex].read_fd)
	{
		Log(CERR, "######  threadIndex = %d", threadIndex);
		return;
	}

	char buf[sizeof(PlatformSocketInfo) * MAX_POST_CONNECTED_COUNT] = "";

	int realAllSize = recv(readfd, buf, sizeof(buf), 0);
	if (realAllSize < sizeof(PlatformSocketInfo) || realAllSize % sizeof(PlatformSocketInfo) != 0)
	{
		Log(CERR, "ThreadLibeventProcess error size=%d,sizeof(PlatformSocketInfo)=%lld",
			realAllSize, sizeof(PlatformSocketInfo));
		event_add(pThis->m_workBaseVec[threadIndex].event, nullptr);
		return;
	}

	int countAcceptCount = realAllSize / sizeof(PlatformSocketInfo);
	for (int i = 0; i < countAcceptCount; i++)
	{
		PlatformSocketInfo* pTCPSocketInfo = (PlatformSocketInfo*)(buf + i * sizeof(PlatformSocketInfo));

		// 处理连接
		pThis->AddTCPSocketInfo(threadIndex, pTCPSocketInfo);
	}

	event_add(pThis->m_workBaseVec[threadIndex].event, nullptr);
}
int  CTCPSocketManage::AddServerSocketInfo(int threadIndex, PlatformSocketInfo* pTCPSocketInfo)
{
	struct event_base* base = m_workBaseVec[threadIndex].base;
	struct bufferevent* bev = nullptr;
	SOCKFD fd = pTCPSocketInfo->acceptFd;

	// 分配索引算法
	int index = GetSocketIndex();
	if (index < 0)
	{
		Log(CERR, "分配索引失败！！！fd=%d,ip=%s", fd, pTCPSocketInfo->ip);
		closesocket(fd);
		return index;
	}
	bev = bufferevent_socket_new(base, fd, /*BEV_OPT_CLOSE_ON_FREE | */BEV_OPT_THREADSAFE);
	if (!bev)
	{
		Log(CERR, "Error constructing bufferevent!,fd=%d,ip=%s", fd, pTCPSocketInfo->ip);
		closesocket(fd);
		return index;
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
		Log(CERR, "add event fail!!!,fd=%d,ip=%s", fd, pTCPSocketInfo->ip);
		closesocket(fd);
		bufferevent_free(bev);
		SafeDelete(pRecvThreadParam);
		return index;
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
		Log(CERR, "分配索引失败,fd=%d,ip=%s", fd, pTCPSocketInfo->ip);
		closesocket(fd);
		bufferevent_free(bev);
		SafeDelete(pRecvThreadParam);
		m_ConditionVariable.GetMutex().unlock(); //解锁
		return index;
	}
	m_socketInfoVec[index] = tcpInfo;
	m_heartBeatSocketSet.insert((unsigned int)index);
	m_uCurSocketSize++;
	m_ConditionVariable.GetMutex().unlock(); //解锁

	return index;
}
void CTCPSocketManage::AddTCPSocketInfo(int threadIndex, PlatformSocketInfo* pTCPSocketInfo)
{
	struct event_base* base = m_workBaseVec[threadIndex].base;
	struct bufferevent* bev = nullptr;
	SOCKFD fd = pTCPSocketInfo->acceptFd;
#ifdef __WebSocketOpenssl__
	SSL* ssl = nullptr;
#endif

	// 分配索引算法
	int index = GetSocketIndex();
	if (index < 0)
	{
		Log(CERR, "分配索引失败！！！fd=%d,ip=%s", fd, pTCPSocketInfo->ip);
		closesocket(fd);
		return;
	}
	if (m_ServiceType == ServiceType::SERVICE_TYPE_LOGIC_WSS)
	{
#ifdef __WebSocketOpenssl__
		ssl = SSL_new(m_ctx);
		if (!ssl)
		{
			Log(CERR, "SSL_new null fd=%d,ip=%s", fd, pTCPSocketInfo->ip);
			return;
		}
		bev = bufferevent_openssl_socket_new(base, fd, ssl, BUFFEREVENT_SSL_ACCEPTING,
			/*BEV_OPT_CLOSE_ON_FREE | */BEV_OPT_THREADSAFE/* | BEV_OPT_DEFER_CALLBACKS*/);
#endif
	}
	else
	{
		bev = bufferevent_socket_new(base, fd, /*BEV_OPT_CLOSE_ON_FREE | */BEV_OPT_THREADSAFE);
	}
	if (!bev)
	{
		Log(CERR, "Error constructing bufferevent!,fd=%d,ip=%s", fd, pTCPSocketInfo->ip);
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
	bufferevent_setcb(bev, ReadCB, nullptr, EventCB, (void*)pRecvThreadParam);
	if (bufferevent_enable(bev, EV_READ | EV_ET) < 0)
	{
		Log(CERR, "add event fail!!!,fd=%d,ip=%s", fd, pTCPSocketInfo->ip);
		closesocket(fd);
		bufferevent_free(bev);
		SafeDelete(pRecvThreadParam);
		return;
	}

	// 设置读超时，当做心跳。网关服务器才需要
	if (m_ServiceType == ServiceType::SERVICE_TYPE_LOGIC ||
		m_ServiceType == ServiceType::SERVICE_TYPE_LOGIC_WS ||
		m_ServiceType == ServiceType::SERVICE_TYPE_LOGIC_WSS)
	{
		timeval tvRead;
		tvRead.tv_sec = CHECK_HEAETBEAT_SECS * KEEP_ACTIVE_HEARTBEAT_COUNT;
		tvRead.tv_usec = 0;
		bufferevent_set_timeouts(bev, &tvRead, nullptr);
	}

	// 保存信息
	TCPSocketInfo tcpInfo;
	memcpy(tcpInfo.ip, pTCPSocketInfo->ip, sizeof(tcpInfo.ip));
	tcpInfo.acceptFd = pTCPSocketInfo->acceptFd;
	tcpInfo.acceptMsgTime = pTCPSocketInfo->acceptMsgTime;
	tcpInfo.bev = bev;
	tcpInfo.isConnect = true;
	tcpInfo.port = pTCPSocketInfo->port;
	tcpInfo.link = G_Util->CreateUserId() + (uint64_t)Util::GetRandNum();
	if (!tcpInfo.lock)
	{
		tcpInfo.lock = new std::mutex;
	}
	tcpInfo.bHandleAccptMsg = false;

	m_ConditionVariable.GetMutex().lock();	//加锁
	if (m_socketInfoVec[index].isConnect)
	{
		Log(CERR, "分配索引失败,fd=%d,ip=%s", fd, pTCPSocketInfo->ip);
		closesocket(fd);
		bufferevent_free(bev);
		SafeDelete(pRecvThreadParam);
		m_ConditionVariable.GetMutex().unlock(); //解锁
		return;
	}
	m_socketInfoVec[index] = tcpInfo;
	m_heartBeatSocketSet.insert((unsigned int)index);
	m_uCurSocketSize++;
	m_ConditionVariable.GetMutex().unlock(); //解锁

	if (m_ServiceType == ServiceType::SERVICE_TYPE_LOGIC_WS)
	{
		return;
	}
	else if (m_ServiceType == ServiceType::SERVICE_TYPE_LOGIC_WSS)
	{
#ifdef __WebSocketOpenssl__
		TCPSocketInfo* tcpInfo1 = GetTCPSocketInfo(index);
		if (tcpInfo1)
		{
			tcpInfo1->ssl = ssl;
		}
#endif
		return;
	}
	else if (m_ServiceType == ServiceType::SERVICE_TYPE_LOGIC)
	{
		// TCP服务器 验证客户端
		Netmsg msg; msg << tcpInfo.link;
		SendMsg(index, msg.str().c_str(), msg.str().size(), MsgCmd::MsgCmd_Testlink, 0, 0, tcpInfo.bev);
		return;
	}
	else if (m_ServiceType == ServiceType::SERVICE_TYPE_CROSS)
	{
		Log(CINF, "TCP connect [ip=%s port=%d index=%d fd=%d bufferevent=%p]",
			tcpInfo.ip, tcpInfo.port, index, tcpInfo.acceptFd, tcpInfo.bev);
		return;
	}
	return;
}

void CTCPSocketManage::ListenerCB(evconnlistener* listener, evutil_socket_t fd, sockaddr* sa, int socklen, void* data)
{
	CTCPSocketManage* pThis = (CTCPSocketManage*)data;

	// 获取连接信息
	struct sockaddr_in* addrClient = (struct sockaddr_in*)sa;
	PlatformSocketInfo tcpInfo;
	tcpInfo.acceptMsgTime = time(nullptr);
	strcpy(tcpInfo.ip, inet_ntoa(addrClient->sin_addr));
	tcpInfo.port = ntohs(addrClient->sin_port);
	tcpInfo.acceptFd = fd;	//服务器accept返回套接字用来和客户端通信

	if (pThis->GetCurSocketSize() >= pThis->m_uMaxSocketSize)
	{
		Log(CERR, "服务器已经满：fd=%d [ip:%s %d][人数：%u/%u]", fd,
			tcpInfo.ip, tcpInfo.port, pThis->GetCurSocketSize(), pThis->m_uMaxSocketSize);

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
	if (send(pThis->m_workBaseVec[lastThreadIndex].write_fd,
		(const char*)(&tcpInfo), sizeof(tcpInfo), 0) < sizeof(tcpInfo))
	{
		Log(CERR, "投递连接消息失败,fd=%d", fd);
	}

	lastThreadIndex++;
}
void CTCPSocketManage::ReadCB(bufferevent* bev, void* data)
{
	RecvThreadParam* param = (RecvThreadParam*)data;
	CTCPSocketManage* pThis = param->pThis;
	int index = param->index;

	// 处理数据，包头解析
	pThis->RecvData(bev, index);
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
		Log(INF, "心跳踢人 index=%d fd=%d", index, pThis->m_socketInfoVec[index].acceptFd);
	}
	else
	{
		Log(CERR, "Got an error on the connection,events=%d", events);
	}

	pThis->RemoveTCPSocketStatus(index, true);
}
void CTCPSocketManage::AcceptErrorCB(evconnlistener* listener, void* data)
{
	Log(CERR, "accept error:%s", evutil_socket_error_to_string(EVUTIL_SOCKET_ERROR()));
}

// 测试连接
bool CTCPSocketManage::VerifyConnection(int index, char* data)
{
	if (!data)
	{
		return false;
	}
	Netmsg msg(data);
	std::string str;
	msg >> str;
	if (str.empty())
	{
		return false;
	}
	TCPSocketInfo* tcpInfo = GetTCPSocketInfo(index);
	if (!tcpInfo)
	{
		return false;
	}
	Util::Decrypt((char*)str.c_str(), str.size());
	if (std::to_string(tcpInfo->link) != str)
	{
		return false;
	}
	
	tcpInfo->link = (uint64_t)MsgCmd::MsgCmd_Testlink;

	Log(CINF, "TCP connect [ip=%s port=%d index=%d fd=%d bufferevent=%p]",
		tcpInfo->ip, tcpInfo->port, index, tcpInfo->acceptFd, tcpInfo->bev);

	return true;
}

// 网络消息派发
bool CTCPSocketManage::DispatchPacket(void* pBufferevent, int index, NetMessageHead* pHead, void* pData, int size,
	SocketType socketType/* = SocketType::SOCKET_TYPE_TCP*/)
{
	if (!pBufferevent || !pHead)
	{
		return false;
	}
	if (m_ServiceType == ServiceType::SERVICE_TYPE_CROSS)
	{
		return DispatchCrossPacket(pBufferevent, index, pHead, pData, size, socketType);
	}
	else
	{
		return DispatchLogicPacket(pBufferevent, index, pHead, pData, size, socketType);
	}
}
// 跨服消息处理
bool CTCPSocketManage::DispatchCrossPacket(void* pBufferevent, int index, NetMessageHead* pHead, void* pData, int size,
	SocketType socketType/* = SocketType::SOCKET_TYPE_TCP*/)
{
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
	msg.socketType = socketType;

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
// 本服消息处理
bool CTCPSocketManage::DispatchLogicPacket(void* pBufferevent, int index, NetMessageHead* pHead, void* pData, int size,
	SocketType socketType/* = SocketType::SOCKET_TYPE_TCP*/)
{
	TCPSocketInfo* pTcpInfo = GetTCPSocketInfo(index);
	if (!pTcpInfo)
	{
		return false;
	}
	if (pHead->uMainID == (unsigned int)MsgCmd::MsgCmd_HeartBeat) //心跳包
	{
		return true;
	}
	if (pHead->uMainID == (unsigned int)MsgCmd::MsgCmd_Testlink) //测试连接包
	{
		if (!VerifyConnection(index, (char*)pData))
		{
			RemoveTCPSocketStatus(index);
		}
		return true;
	}
	if (m_ServiceType != ServiceType::SERVICE_TYPE_CROSS && pTcpInfo->isCross)
	{
		return MsgForward(index, pHead, (char*)pData);
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
	msg.socketType = socketType;

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

void TCPSocketInfo::Reset(ServiceType& serviceType)
{
	// 和发送线程相关的锁
	lock->lock();

	isConnect = false;
	bufferevent_free(bev);
	bev = nullptr;
	bHandleAccptMsg = false;
	link = 0;
	if (serviceType == ServiceType::SERVICE_TYPE_LOGIC_WSS)
	{
#ifdef __WebSocketOpenssl__
		SSL_shutdown(ssl);
		SSL_free(ssl);
		ssl = nullptr;
#endif
	}
	isCross = false;

	// 解锁发送线程
	lock->unlock();
}
//网络关闭处理
bool CTCPSocketManage::OnSocketCloseEvent(unsigned long uAccessIP, unsigned int uIndex, unsigned int uConnectTime)
{
	SocketCloseLine SocketClose;
	SocketClose.uConnectTime = uConnectTime;
	SocketClose.uIndex = uIndex;
	SocketClose.uAccessIP = uAccessIP;
	return (m_pRecvDataLine->AddData(&SocketClose, sizeof(SocketClose), SysMsgCmd::HD_SOCKET_CLOSE) != 0);
}
bool CTCPSocketManage::CloseSocket(int index)
{
	RemoveTCPSocketStatus(index);

	return true;
}
void CTCPSocketManage::RemoveTCPSocketStatus(int index, bool isClientAutoClose/* = false*/)
{
	TCPSocketInfo* tcpInfo = GetTCPSocketInfo(index);
	if (!tcpInfo)
	{
		Log(CERR, "index=%d 超出范围", index);
		return;
	}

	unsigned long uAccessIP = 0;

	// 加锁
	m_ConditionVariable.GetMutex().lock();
	// 重复调用
	if (!tcpInfo->isConnect)
	{
		return;
	}
	// 如果锁没有分配内存，就分配
	if (!tcpInfo->lock)
	{
		tcpInfo->lock = new std::mutex;
	}
	uAccessIP = inet_addr(tcpInfo->ip);
	m_uCurSocketSize--;
	m_heartBeatSocketSet.erase((unsigned int)index);
	// 释放参数内存
	RecvThreadParam* pRecvThreadParam = (RecvThreadParam*)0x01;
	bufferevent_getcb(tcpInfo->bev, nullptr, nullptr, nullptr, (void**)&pRecvThreadParam);
	if (pRecvThreadParam)
	{
		SafeDelete(pRecvThreadParam);
	}
	// 和发送线程相关的锁
	tcpInfo->Reset(m_ServiceType);
	// 解锁多线程
	m_ConditionVariable.GetMutex().unlock();

	// 如果没有设置BEV_OPT_CLOSE_ON_FREE 选项，则关闭socket
	closesocket(tcpInfo->acceptFd);
	OnSocketCloseEvent(uAccessIP, index, (unsigned int)tcpInfo->acceptMsgTime);

	// 清理登录内存
	G_PlayerPrepClient->GetLoginSys().DelLoginInMap(index);

	if (IsServerMsg(index))
	{
		Log(CERR, "服务器异常断开链接请检查游戏逻辑 index=%d",index);
	}
	else
	{
		Log(CINF, "TCP close [ip=%s port=%d index=%d fd=%d isClientAutoClose:%d acceptTime=%lld]",
			tcpInfo->ip, tcpInfo->port, index, tcpInfo->acceptFd, isClientAutoClose, tcpInfo->acceptMsgTime);
	}
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
int CTCPSocketManage::GetDBServerIndex()
{
	return m_DBServerIndex;
}
int CTCPSocketManage::GetCrossServerIndex()
{
	return m_CrossServerIndex;
}
void CTCPSocketManage::GetSocketSet(std::vector<unsigned int>& vec)
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
	TCPSocketInfo* tcpInfo = GetTCPSocketInfo(index);
	if (!tcpInfo)
	{
		return nullptr;
	}

	return tcpInfo->ip;
}
TCPSocketInfo* CTCPSocketManage::GetTCPSocketInfo(int index)
{
	if (index < 0 || index >= m_socketInfoVec.size())
	{
		return nullptr;
	}

	return &m_socketInfoVec[index];
}
bool& CTCPSocketManage::GetRuninged()
{
	return m_running;
}
ServiceType CTCPSocketManage::GetServerType()
{
	return m_ServiceType;
}
ConditionVariable& CTCPSocketManage::GetConditionVariable()
{
	return m_ConditionVariable;
}
event_base* CTCPSocketManage::GetEventBase()
{
	return m_listenerBase;
}
std::vector<std::thread*>& CTCPSocketManage::GetSockeThreadVec()
{
	return m_socketThread;
}

bool CTCPSocketManage::IsConnected(int index)
{
	TCPSocketInfo* tcpInfo = GetTCPSocketInfo(index);
	if (!tcpInfo)
	{
		return false;
	}
	return tcpInfo->isConnect;
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

void CTCPSocketManage::SetTcpRcvSndBUF(SOCKFD fd, int rcvBufSize, int sndBufSize)
{
	int optval = 0;
#if defined(_WIN32)
	int optLen = sizeof(int);
#elif defined(_WIN64)
#elif defined(__linux__)
	socklen_t optLen = sizeof(int);
#elif defined(__unix__)
#elif defined(__ANDROID__)
#elif defined(__APPLE__)
#endif

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
		Log(CERR, "bufferevent_set_max_single_read fail,bev=%p", bev);
	}

	/*if (bufferevent_set_max_single_write(bev, sndBufSize) < 0)
	{
		Log(CERR, "bufferevent_set_max_single_write fail,fd=%d", fd);
	}*/
}

int CTCPSocketManage::StreamSocketpair(struct addrinfo* addr_info, SOCKFD sock[2])
{
	if (!addr_info)
	{
		return -1;
	}

	SOCKFD listener, client, server;
	int opt = 1;

	listener = server = client = INVALID_SOCKET;
	listener = socket(addr_info->ai_family, addr_info->ai_socktype, addr_info->ai_protocol); //创建服务器socket并进行绑定监听等
	if (INVALID_SOCKET == listener)
	{
		goto fail;
	}

	setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));

	if (SOCKET_ERROR == ::bind(listener, addr_info->ai_addr, static_cast<int>(addr_info->ai_addrlen)))
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
int CTCPSocketManage::DgramSocketpair(struct addrinfo* addr_info, SOCKFD sock[2])
{
	if (!addr_info)
	{
		return -1;
	}
	SOCKFD client, server;
	struct addrinfo addr, * result = nullptr;
	const char* address;
	int opt = 1;

	server = client = INVALID_SOCKET;

	server = socket(addr_info->ai_family, addr_info->ai_socktype, addr_info->ai_protocol);
	if (INVALID_SOCKET == server)
	{
		goto fail;
	}

	setsockopt(server, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));

	if (SOCKET_ERROR == ::bind(server, addr_info->ai_addr, static_cast<int>(addr_info->ai_addrlen)))
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

	if (SOCKET_ERROR == ::bind(client, result->ai_addr, static_cast<int>(result->ai_addrlen)))
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
int CTCPSocketManage::Socketpair(int family, int type, int protocol, SOCKFD recv[2])
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

// 消息发送
bool CTCPSocketManage::BuffereventWrite(int index, void* data, unsigned int size)
{
	//发送数据
	TCPSocketInfo* tcpInfo = GetTCPSocketInfo(index);
	if (!tcpInfo)
	{
		Log(CERR, "index=%d 超出范围", index);
		return false;
	}
	if (!tcpInfo->lock)
	{
		return false;
	}
	{
		std::lock_guard<std::mutex> guard(*tcpInfo->lock);
		if (tcpInfo->isConnect && tcpInfo->bev)
		{
			if (bufferevent_write(tcpInfo->bev, data, size) < 0)
			{
				Log(CERR, "发送数据失败，index=%d socketfd=%d bev=%p,", index, tcpInfo->acceptFd, tcpInfo->bev);
			}
		}
	}

	return true;
}
bool CTCPSocketManage::SendMsg(int index, const char* pData, size_t size, MsgCmd mainID, int assistID, int handleCode, 
	void* pBufferevent, unsigned int uIdentification/* = 0*/, bool WSPackData/* = true*/)
{
	if (IsServerMsg(index))
	{
		return SendLogicMsg(index, pData, size, mainID, assistID, handleCode, pBufferevent, uIdentification);
	}
	else
	{
		if (m_ServiceType == ServiceType::SERVICE_TYPE_LOGIC_WS)
		{
#ifdef __WebSocket__
			return SendLogicWsMsg(index, pData, size, mainID, assistID, handleCode, pBufferevent, uIdentification, WSPackData);
#endif
		}
		else if (m_ServiceType == ServiceType::SERVICE_TYPE_LOGIC_WSS)
		{
#ifdef __WebSocketOpenssl__
			return SendLogicWssMsg(index, pData, size, mainID, assistID, handleCode, pBufferevent, uIdentification, WSPackData);
#endif
		}
		else
		{
			return SendLogicMsg(index, pData, size, mainID, assistID, handleCode, pBufferevent, uIdentification);
		}
	}

	return true;
}
bool CTCPSocketManage::SendLogicMsg(int index, const char* pData, size_t size, MsgCmd mainID, int assistID, int handleCode, 
	void* pBufferevent, unsigned int uIdentification/* = 0*/)
{
	if (!pBufferevent)
	{
		Log(CERR, "!pBufferevent");
		return false;
	}
	if (!IsConnected(index))
	{
		Log(CERR, "socketIdx close, index=%d, mainID=%d assistID=%d", index, mainID, assistID);
		return false;
	}

	if (size < 0 || size > MAX_TEMP_SENDBUF_SIZE - sizeof(NetMessageHead))
	{
		Log(CERR, "invalid message size size=%lld", size);
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
			Log(CERR, "投递消息失败,mainID=%d,assistID=%d", mainID, assistID);
			return false;
		}
	}

	return true;
}
#ifdef __WebSocket__
bool CTCPSocketManage::SendLogicWsMsg(int index, const char* pData, size_t size, MsgCmd mainID, int assistID, int handleCode, 
	void* pBufferevent, unsigned int uIdentification/* = 0*/, bool PackData/* = true*/)
{
	if (!pBufferevent)
	{
		Log(CERR, "!pBufferevent");
		return false;
	}
	if (!IsConnected(index))
	{
		Log(CERR, "socketIdx close, index=%d, mainID=%d assistID=%d", index, mainID, assistID);
		return false;
	}
	// 不包装数据
	if (!PackData)
	{
		if (size < 0 || size > MAX_TEMP_SENDBUF_SIZE)
		{
			Log(CERR, "invalid message size size=%lld", size);
			return false;
		}
		// 整合一下数据
		std::unique_ptr<char[]> SendBuf(new char[sizeof(SendDataLineHead) + size]);

		// 包体
		if (pData && size > 0)
		{
			memcpy(SendBuf.get() + sizeof(SendDataLineHead), pData, size);
		}

		// 投递到发送队列
		if (m_pSendDataLine)
		{
			SendDataLineHead* pLineHead = reinterpret_cast<SendDataLineHead*>(SendBuf.get());
			pLineHead->dataLineHead.uSize = (unsigned int)size;
			pLineHead->socketIndex = index;
			pLineHead->pBufferevent = pBufferevent;

			unsigned int addBytes = m_pSendDataLine->AddData(pLineHead, sizeof(SendDataLineHead) + (unsigned int)size);

			if (addBytes == 0)
			{
				Log(CERR, "投递消息失败,mainID=%d,assistID=%d", mainID, assistID);
				return false;
			}
		}
	}
	// 包装数据
	else
	{
		if (size < 0 || size > MAX_TEMP_SENDBUF_SIZE - sizeof(NetMessageHead))
		{
			Log(CERR, "invalid message size size=%lld", size);
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
				Log(CERR, "投递消息失败,mainID=%d,assistID=%d", mainID, assistID);
				return false;
			}
		}
	}

	return true;
}
#endif
#ifdef __WebSocketOpenssl__
bool CTCPSocketManage::SendLogicWssMsg(int index, const char* pData, size_t size, MsgCmd mainID, int assistID, int handleCode,
	void* pBufferevent, unsigned int uIdentification/* = 0*/, bool PackData/* = true*/)
{
	return SendLogicWsMsg(index, pData, size, mainID, assistID, handleCode, pBufferevent, uIdentification, PackData);
}
#endif

// 发送线程消息处理发送
void CTCPSocketManage::ThreadSendMsg()
{
	std::list <ListItemData*> dataList;
	CDataLine* pDataLine = GetSendDataLine();
	if (!pDataLine)
	{
		Log(CERR, "send list is null");
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
			HandleSendMsg(dataList.front());
			dataList.pop_front();
		}
	}

	Log(CINF, "send data thread end");

	return;
}
void CTCPSocketManage::HandleSendMsg(ListItemData* pListItem)
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
	if (!pSocketSend)
	{
		return;
	}
	if (IsServerMsg(pSocketSend->socketIndex))
	{
		HandleSendData(pListItem);
	}
	else
	{
		if (m_ServiceType == ServiceType::SERVICE_TYPE_LOGIC_WS)
		{
#ifdef __WebSocket__
			HandleSendWsData(pListItem);
#endif
		}
		else if (m_ServiceType == ServiceType::SERVICE_TYPE_LOGIC_WSS)
		{
#ifdef __WebSocketOpenssl__
			HandleSendWssData(pListItem);
#endif
		}
		else
		{
			HandleSendData(pListItem);
		}
	}

	SafeDeleteArray(pListItem->pData);
	SafeDelete(pListItem);
}
void CTCPSocketManage::HandleSendData(ListItemData* pListItem)
{
	SendDataLineHead* pSocketSend = reinterpret_cast<SendDataLineHead*>(pListItem->pData);
	unsigned int size = pSocketSend->dataLineHead.uSize;
	int index = pSocketSend->socketIndex;
	void* pData = pListItem->pData + sizeof(SendDataLineHead);

	if (!BuffereventWrite(index, pData, size))
	{
		return;
	}
}
#ifdef __WebSocket__
void CTCPSocketManage::HandleSendWsData(ListItemData* pListItem)
{
	SendDataLineHead* pSocketSend = reinterpret_cast<SendDataLineHead*>(pListItem->pData);
	unsigned int size = pSocketSend->dataLineHead.uSize;
	int index = pSocketSend->socketIndex;
	void* pData = pListItem->pData + sizeof(SendDataLineHead);

	TCPSocketInfo* tcpInfo = GetTCPSocketInfo(index);
	if (!tcpInfo)
	{
		Log(CERR, "index=%d 超出范围", index);
		return;
	}
	// websocket 握手
	if (!tcpInfo->bHandleAccptMsg)
	{
		//发送数据
		if (!tcpInfo->lock)
		{
			return;
		}
		{
			std::lock_guard<std::mutex> guard(*tcpInfo->lock);
			if (tcpInfo->isConnect && tcpInfo->bev)
			{
				if (bufferevent_write(tcpInfo->bev, pData, size) < 0)
				{
					Log(CERR, "发送数据失败，index=%d socketfd=%d bev=%p,", index, tcpInfo->acceptFd, tcpInfo->bev);
				}
				// 标记已经处理握手
				tcpInfo->bHandleAccptMsg = true;
			}
		}
		if (tcpInfo->bHandleAccptMsg)
		{
			// websocket服务器 握手成功 验证客户端
			Netmsg msg; msg << tcpInfo->link;
			SendMsg(index, msg.str().c_str(), msg.str().size(), MsgCmd::MsgCmd_Testlink, 0, 0, tcpInfo->bev);
		}
		return;
	}
	// websocket 握手成功
	else
	{
		unsigned int* payload_len_32_be = nullptr;
		unsigned short int* payload_len_short_be = nullptr;
		unsigned char finNopcode{}, payload_len_small{};
		unsigned int payload_offset = 2;
		unsigned int frame_size = 0;
		int flags = WS_FRAGMENT_FIN | WS_BINARY_FRAME;

		finNopcode = flags & 0xff;

		int allLen = sizeof(NetMessageHead) + size;// PACKET_HEADER_SIZE;
		if (allLen <= 125)
		{
			frame_size = 2 + allLen;
			payload_len_small = allLen & 0xff;
		}
		else if (allLen > 125 && allLen <= 0xffff)
		{
			frame_size = 4 + allLen;
			payload_len_small = 126;
			payload_offset += 2;
		}
		else if (allLen > 0xffff && allLen <= 0xfffffff0)
		{
			frame_size = 10 + allLen;
			payload_len_small = 127;
			payload_offset += 8;
		}
		else
		{
			Log(CERR, "web socket send to big size=%d", size);
			return;
		}

		// 整合一下数据
		std::unique_ptr<char[]> recvBuf(new char[frame_size]);
		char* frame = recvBuf.get();

		// 填充websocket包头
		payload_len_small &= 0x7f;
		*frame = finNopcode;
		*(frame + 1) = payload_len_small;
		if (payload_len_small == 126)
		{
			allLen &= 0xffff;
			payload_len_short_be = (unsigned short*)((char*)frame + 2);
			*payload_len_short_be = htons(allLen);
		}
		if (payload_len_small == 127)
		{
			payload_len_32_be = (unsigned int*)((char*)frame + 2);
			*payload_len_32_be++ = 0;
			*payload_len_32_be = htonl(allLen);
		}
		// 应用层包体
		if (pData && size > 0)
		{
			memcpy(frame + payload_offset, pData, size);
		}
		if (!BuffereventWrite(index, frame, frame_size))
		{
			return;
		}
		return;
	}
	return;
}
#endif
#ifdef __WebSocketOpenssl__
void CTCPSocketManage::HandleSendWssData(ListItemData* pListItem)
{
	HandleSendWsData(pListItem);
}
#endif

// 接收消息进行解包处理
bool CTCPSocketManage::RecvData(bufferevent* bev, int index)
{
	if (IsServerMsg(index))
	{
		if (!RecvLogicData(bev, index))
		{
			return false;
		}
	}
	else
	{
		if (m_ServiceType == ServiceType::SERVICE_TYPE_LOGIC_WS)
		{
#ifdef __WebSocket__
			if (!RecvLogicWsData(bev, index))
			{
				return false;
			}
#endif
		}
		else if (m_ServiceType == ServiceType::SERVICE_TYPE_LOGIC_WSS)
		{
#ifdef __WebSocketOpenssl__
			if (!RecvLogicWssData(bev, index))
			{
				return false;
			}
#endif
		}
		else
		{
			if (!RecvLogicData(bev, index))
			{
				return false;
			}
		}
	}
	
	return true;
}
bool CTCPSocketManage::RecvLogicData(bufferevent* bev, int index)
{
	if (bev == nullptr)
	{
		Log(CERR, "RecvData error bev == nullptr");
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
		Log(CERR, "消息格式不正确,index=%d", index);
		CloseSocket(index);
		return false;
	}
	// 粘包处理
	while (handleRemainSize >= sizeof(NetMessageHead) && handleRemainSize >= pNetHead->uMessageSize)
	{
		unsigned int messageSize = pNetHead->uMessageSize;
		if (messageSize > MAX_TEMP_SENDBUF_SIZE)
		{
			// 消息格式不正确
			Log(CERR, "消息格式不正确");
			CloseSocket(index);
			return false;
		}
		int realSize = messageSize - sizeof(NetMessageHead);
		if (realSize < 0)
		{
			// 数据包不够包头
			Log(CERR, "数据包不够包头");
			CloseSocket(index);
			return false;
		}
		void* pData = nullptr;
		if (realSize > 0)
		{
			// 没数据就为nullptr
			pData = (void*)(recvBuf.get() + realAllSize - handleRemainSize + sizeof(NetMessageHead));
		}
		// 派发数据
		DispatchPacket(bev, index, pNetHead, pData, realSize, SocketType::SOCKET_TYPE_TCP);
		handleRemainSize -= messageSize;
		pNetHead = (NetMessageHead*)(recvBuf.get() + realAllSize - handleRemainSize);
	}

	evbuffer_drain(input, realAllSize - handleRemainSize);
	return true;
}
#ifdef __WebSocket__
bool CTCPSocketManage::RecvLogicWsData(bufferevent* bev, int index)
{
	if (bev == nullptr)
	{
		Log(CERR, "RecvData error bev == nullptr");
		return false;
	}
	TCPSocketInfo* tcpInfo = GetTCPSocketInfo(index);
	if (!tcpInfo)
	{
		Log(CERR, "index=%d 超出范围", index);
		return false;
	}
	if (!tcpInfo->bHandleAccptMsg)
	{
		return HandShark(bev, index);
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

	// 不够一个包头
	if (handleRemainSize < MIN_WEBSOCKET_HEAD_SIZE + sizeof(NetMessageHead))
	{
		return true;
	}

	char* pBuffer = recvBuf.get();
	WebSocketMsg wbmsg;

	// 粘包处理，每一个循环处理一个数据包
	while (true)
	{
		// 不够一个包头
		if (handleRemainSize < MIN_WEBSOCKET_HEAD_SIZE + sizeof(NetMessageHead))
		{
			break;
		}

		// 解析websocket包头
		int pos = 0;
		wbmsg.Init();
		FetchFin(pBuffer, pos, wbmsg);
		FetchOpcode(pBuffer, pos, wbmsg);
		FetchMask(pBuffer, pos, wbmsg);
		FetchPayloadLength(pBuffer, pos, wbmsg);
		FetchMaskingKey(pBuffer, pos, wbmsg);

		if (wbmsg.dataLength > SOCKET_RECV_BUF_SIZE)
		{
			// 消息格式不正确
			Log(CERR, "消息格式不正确,index=%d,maxsize=%u,wbmsg.dataLength=%u",
				index, SOCKET_RECV_BUF_SIZE, wbmsg.dataLength);
			CloseSocket(index);
			return false;
		}

		// 数据不够一个完整的包，不继续处理
		if (handleRemainSize < wbmsg.dataLength)
		{
			break;
		}

		FetchPayload(pBuffer, pos, wbmsg);
		//WSFetchPrint(wbmsg);

		// 解析应用层包头
		NetMessageHead* pNetHead = (NetMessageHead*)wbmsg.payload;

		// 协议格式判断
		if (pNetHead->uMessageSize > SOCKET_RECV_BUF_SIZE || pNetHead->uMessageSize != wbmsg.payloadLength)
		{
			// 消息格式不正确
			CloseSocket(index);
			Log(CERR, "消息格式不正确,index=%d,pNetHead->uMessageSize=%u,wbmsg.payloadLength=%u",
				index, pNetHead->uMessageSize, wbmsg.payloadLength);
			return false;
		}

		unsigned int messageSize = pNetHead->uMessageSize;
		if (messageSize > MAX_TEMP_SENDBUF_SIZE)
		{
			// 消息格式不正确
			CloseSocket(index);
			Log(CERR, "消息格式不正确,index=%d,messageSize=%u", index, messageSize);
			return false;
		}

		int realSize = messageSize - sizeof(NetMessageHead);
		if (realSize < 0)
		{
			// 数据包不够包头
			CloseSocket(index);
			Log(CERR, "数据包不够包头,index=%d,realSize=%d", index, realSize);
			return false;
		}

		void* pData = NULL;
		if (realSize > 0)
		{
			// 没数据就为NULL
			pData = (void*)(wbmsg.payload + sizeof(NetMessageHead));
		}

		// 派发数据
		DispatchPacket(bev, index, pNetHead, pData, realSize, SocketType::SOCKET_TYPE_WEBSOCKET);

		handleRemainSize -= pos;

		pBuffer = pBuffer + pos;
	}

	// 删除buffer数据
	evbuffer_drain(input, realAllSize - handleRemainSize);

	return true;
}
#endif
#ifdef __WebSocketOpenssl__
bool CTCPSocketManage::RecvLogicWssData(bufferevent* bev, int index)
{
	return RecvLogicWsData(bev, index);
}
#endif

// 进行openssl握手
#ifdef __WebSocketOpenssl__
bool CTCPSocketManage::OpensslInit()
{
	SSL_library_init();//初始化库
	OpenSSL_add_all_algorithms();
	SSL_load_error_strings();//加载错误信息

	m_ctx = SSL_CTX_new(SSLv23_server_method());//SSLv23_server_method or SSLv23_client_method 
	if (nullptr == m_ctx)
	{
		Log(CERR, "SSL_CTX_new error");
		return false;
	}
	SSL_CTX_set_verify(m_ctx, SSL_VERIFY_NONE, nullptr);//取消验证前端证书
	// 设置信任根证书
	if (SSL_CTX_load_verify_locations(m_ctx, CA_CERT_FILE, NULL) <= 0)
	{
		Log(CERR, "SSL_CTX_load_verify_locations kill myself,%s", ERR_error_string(ERR_get_error(), NULL));
		return false;
	}
	if (1 != SSL_CTX_use_certificate_file(m_ctx, SERVER_CERT_FILE, SSL_FILETYPE_PEM))//加载证书
	{
		Log(CERR, "SSL_CTX_use_certificate_file error");
		return false;
	}
	if (1 != SSL_CTX_use_PrivateKey_file(m_ctx, SERVER_KEY_FILE, SSL_FILETYPE_PEM))//加载密钥
	{
		Log(CERR, "SSL_CTX_use_PrivateKey_file error");
		return false;
	}
	if (1 != SSL_CTX_check_private_key(m_ctx))//验证密钥
	{
		Log(CERR, "SSL_CTX_check_private_key error");
		return false;
	}

	return true;
}
#endif

#ifdef __WebSocket__
bool CTCPSocketManage::HandShark(bufferevent* bev, int index)
{
	struct evbuffer* input = bufferevent_get_input(bev);

	size_t maxSingleRead = Min_(evbuffer_get_length(input), MAX_TEMP_SENDBUF_SIZE);

	std::unique_ptr<char[]> recvBuf(new char[maxSingleRead]);

	size_t realAllSize = evbuffer_remove(input, recvBuf.get(), maxSingleRead);
	if (realAllSize <= 0)
	{
		return false;
	}

	std::istringstream s(recvBuf.get());
	std::string request;
	MapStringString headMap;

	std::getline(s, request);
	if (request[request.size() - 1] == '\r')
	{
		request.erase(request.end() - 1);
	}
	else
	{
		CloseSocket(index);
		Log(CINF, "消息格式不正确,request=%s", request.c_str());
		return false;
	}

	std::string header;
	std::string::size_type end;

	while (std::getline(s, header) && header != "\r")
	{
		if (header[header.size() - 1] != '\r')
		{
			continue; //end
		}
		else
		{
			header.erase(header.end() - 1);	//remove last char
		}
		end = header.find(": ", 0);
		if (end != std::string::npos)
		{
			std::string key = header.substr(0, end);
			std::string value = header.substr(end + 2);
			headMap[key] = value;
		}
	}

	int i = 1;
	char requestSend[1024] = { 0 };

	strcat(requestSend, "HTTP/1.1 101 Switching Protocols\r\n");
	strcat(requestSend, "Connection: upgrade\r\n");
	strcat(requestSend, "Sec-WebSocket-Accept: ");
	std::string server_key = headMap["Sec-WebSocket-Key"];

	server_key += MAGIC_KEY;

	CSha1 sha;
	unsigned int message_digest[5];
	sha.Reset();
	sha << server_key.c_str();

	sha.Result(message_digest);
	for (int i = 0; i < 5; i++)
	{
		message_digest[i] = htonl(message_digest[i]);
	}
	server_key = base64_encode(reinterpret_cast<const unsigned char*>(message_digest), 20);
	server_key += "\r\n";
	strcat(requestSend, server_key.c_str());
	strcat(requestSend, "Upgrade: websocket\r\n\r\n");

	std::string requestSendStr = requestSend;

	TCPSocketInfo* tcpInfo = GetTCPSocketInfo(index);
	if (tcpInfo)
	{
		//发送数据
		SendMsg(index, requestSendStr.c_str(), requestSendStr.size(),
			MsgCmd::MsgCmd_HandShark, 0, 0, tcpInfo->bev, 0, tcpInfo->bHandleAccptMsg);
	}
	
	return true;
}
#endif

#ifdef __WebSocket__
int CTCPSocketManage::FetchFin(char* msg, int& pos, WebSocketMsg& wbmsg)
{
	wbmsg.fin = (unsigned char)msg[pos] >> 7;
	return 0;
}
int CTCPSocketManage::FetchOpcode(char* msg, int& pos, WebSocketMsg& wbmsg)
{
	wbmsg.opcode = msg[pos] & 0x0f;
	pos++;
	return 0;
}
int CTCPSocketManage::FetchMask(char* msg, int& pos, WebSocketMsg& wbmsg)
{
	wbmsg.mask = (unsigned char)msg[pos] >> 7;
	return 0;
}
int CTCPSocketManage::FetchMaskingKey(char* msg, int& pos, WebSocketMsg& wbmsg)
{
	if (wbmsg.mask != 1)
	{
		return 0;
	}
	for (int i = 0; i < 4; i++)
	{
		wbmsg.maskingKey[i] = msg[pos + i];
	}
	pos += 4;
	return 0;
}
int CTCPSocketManage::FetchPayloadLength(char* msg, int& pos, WebSocketMsg& wbmsg)
{
	wbmsg.payloadLength = msg[pos] & 0x7f;
	pos++;
	if (wbmsg.payloadLength == 126) 
	{
		uint16_t length = 0;
		memcpy(&length, msg + pos, 2);
		pos += 2;
		wbmsg.payloadLength = ntohs(length);
	}
	else if (wbmsg.payloadLength == 127) 
	{
		uint32_t length = 0;
		memcpy(&length, msg + pos, 4);
		pos += 4;
		wbmsg.payloadLength = ntohl(length);
	}

	wbmsg.dataLength = pos + wbmsg.payloadLength;

	return 0;
}
int CTCPSocketManage::FetchPayload(char* msg, int& pos, WebSocketMsg& wbmsg)
{
	wbmsg.payload = msg + pos;

	if (wbmsg.mask == 1) 
	{
		for (uint32_t i = 0; i < wbmsg.payloadLength; i++) 
		{
			uint32_t j = i % 4;
			wbmsg.payload[i] = msg[pos + i] ^ wbmsg.maskingKey[j];
		}
	}

	pos += wbmsg.payloadLength;

	return 0;
}
void CTCPSocketManage::FetchPrint(const WebSocketMsg& wbmsg)
{
	Log(CINF, "WEBSOCKET PROTOCOL FIN: %d OPCODE: %d MASK: %d DATALEN:%u PAYLOADLEN: %u\n",
		wbmsg.fin, wbmsg.opcode, wbmsg.mask, wbmsg.dataLength, wbmsg.payloadLength);
}
#endif

// 跨服相关
// 跨服消息转发
bool CTCPSocketManage::MsgForward(int index, NetMessageHead* pHead, char* pData)
{
	int crossIndex = GetCrossServerIndex();
	if (crossIndex <= 0)
	{
		return false;
	}
	TCPSocketInfo* pCrossTcpInfo = GetTCPSocketInfo(crossIndex);
	if (!pCrossTcpInfo)
	{
		return false;
	}

	PlayerClient* player = G_PlayerCenterClient->GetPlayerClientByIndex(index);
	if (player)
	{
		Netmsg msg;
		msg << G_CfgMgr->GetCBaseCfgMgr().GetServerId()
			<< player->GetID()
			<< pData;

		SendMsg(crossIndex, msg.str().c_str(), msg.str().size(), (MsgCmd)pHead->uMainID,
			pHead->uAssistantID, pHead->uHandleCode, pCrossTcpInfo->bev, pHead->uIdentification);
	}

	return true;
}
