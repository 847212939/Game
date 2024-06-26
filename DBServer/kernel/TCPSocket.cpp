#include "../stdafx.h"

TCPSocket::TCPSocket() :
	m_bindIP(""),
	m_running(false),
	m_uMaxSocketSize(0),
	m_uCurSocketSize(0),
	m_uCurSocketIndex(0),
	m_listenerBase(nullptr),
	m_pRecvDataLine(new CDataLine),
	m_pSendDataLine(new CDataLine),
	m_eventBaseCfg(event_config_new()),
	m_port(0)
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
TCPSocket::~TCPSocket()
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

bool TCPSocket::Init(int maxCount, int port, const char* ip, 
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

	return true;
}
bool TCPSocket::Stop()
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
bool TCPSocket::Start()
{
	if (m_running == true)
	{
		Log(CERR, "service tcp already have been running");
		return false;
	}

	m_running = true;
	m_uCurSocketSize = 0;
	m_uCurSocketIndex = 0;

	m_socketThread.emplace_back(new std::thread(&TCPSocket::ThreadSendMsg, this));
	m_socketThread.emplace_back(new std::thread(&TCPSocket::ThreadAccept, this));

	return true;
}

void TCPSocket::ThreadAccept()
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
		LEV_OPT_REUSEABLE | /*LEV_OPT_CLOSE_ON_FREE |*/ LEV_OPT_THREADSAFE,
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

		m_workBaseVec.emplace_back(workInfo);
	}

	std::vector<std::thread> threadVev;
	// 开辟工作线程池
	for (int i = 0; i < workBaseCount; i++)
	{
		threadVev.emplace_back(std::thread(ThreadRSSocket, (void*)&uniqueParam[i]));
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
void TCPSocket::ThreadRSSocket(void* pThreadData)
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
void TCPSocket::ThreadLibeventProcess(evutil_socket_t readfd, short which, void* arg)
{
	RecvThreadParam* param = (RecvThreadParam*)arg;
	TCPSocket* pThis = param->pThis;
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

void TCPSocket::AddTCPSocketInfo(int threadIndex, PlatformSocketInfo* pTCPSocketInfo)
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
		return;
	}

	bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE | BEV_OPT_THREADSAFE);

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
	if (!tcpInfo.lock)
	{
		tcpInfo.lock = new std::mutex;
	}
	tcpInfo.bHandleAccptMsg = false;

	{
		std::lock_guard<std::mutex> guard(m_mutex);
		if (m_socketInfoVec[index].isConnect)
		{
			Log(CERR, "分配索引失败,fd=%d,ip=%s", fd, pTCPSocketInfo->ip);
			closesocket(fd);
			bufferevent_free(bev);
			SafeDelete(pRecvThreadParam);
			return;
		}
		m_socketInfoVec[index] = tcpInfo;
		m_heartBeatSocketSet.emplace((unsigned int)index);
		m_uCurSocketSize++;
	}
	

	Log(CINF, "TCP connect [ip=%s port=%d index=%d fd=%d bufferevent=%p]",
		tcpInfo.ip, tcpInfo.port, index, tcpInfo.acceptFd, tcpInfo.bev);
}

void TCPSocket::ListenerCB(evconnlistener* listener, evutil_socket_t fd, sockaddr* sa, int socklen, void* data)
{
	TCPSocket* pThis = (TCPSocket*)data;

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
void TCPSocket::ReadCB(bufferevent* bev, void* data)
{
	RecvThreadParam* param = (RecvThreadParam*)data;
	TCPSocket* pThis = param->pThis;
	int index = param->index;

	// 处理数据，包头解析
	pThis->RecvData(bev, index);
}
void TCPSocket::EventCB(bufferevent* bev, short events, void* data)
{
	RecvThreadParam* param = (RecvThreadParam*)data;
	TCPSocket* pThis = param->pThis;
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
void TCPSocket::AcceptErrorCB(evconnlistener* listener, void* data)
{
	Log(CERR, "accept error:%s", evutil_socket_error_to_string(EVUTIL_SOCKET_ERROR()));
}

// 网络消息派发
bool TCPSocket::DispatchPacket(void* pBufferevent, int index, NetMessageHead* pHead, void* pData, int size, 
	SocketType socketType/* = SocketType::SOCKET_TYPE_TCP*/)
{
	if (!pBufferevent || !pHead)
	{
		return false;
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
	{
		std::lock_guard<std::mutex> guard(*lock);
		isConnect = false;
		bufferevent_free(bev);
		bev = nullptr;
		bHandleAccptMsg = false;
	}
}
bool TCPSocket::CloseSocket(int index)
{
	RemoveTCPSocketStatus(index);

	return true;
}
void TCPSocket::RemoveTCPSocketStatus(int index, bool isClientAutoClose/* = false*/)
{
	TCPSocketInfo* tcpInfo = GetTCPSocketInfo(index);
	if (!tcpInfo)
	{
		Log(CERR, "index=%d 超出范围", index);
		return;
	}

	unsigned long uAccessIP = 0;

	{
		std::lock_guard<std::mutex> guard(m_mutex);
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
	}

	// 如果没有设置BEV_OPT_CLOSE_ON_FREE 选项，则关闭socket
	closesocket(tcpInfo->acceptFd);

	Log(CINF, "TCP close [ip=%s port=%d index=%d fd=%d isClientAutoClose:%d acceptTime=%lld]",
		tcpInfo->ip, tcpInfo->port, index, tcpInfo->acceptFd, isClientAutoClose, tcpInfo->acceptMsgTime);
}

CDataLine* TCPSocket::GetRecvDataLine()
{
	return m_pRecvDataLine;
}
CDataLine* TCPSocket::GetSendDataLine()
{
	return m_pSendDataLine;
}
unsigned int TCPSocket::GetCurSocketSize()
{
	return m_uCurSocketSize;
}
void TCPSocket::GetSocketSet(std::vector<unsigned int>& vec)
{
	vec.clear();

	std::lock_guard<std::mutex> guard(m_mutex);

	for (auto iter = m_heartBeatSocketSet.begin(); iter != m_heartBeatSocketSet.end(); iter++)
	{
		vec.emplace_back(*iter);
	}
}
const std::vector<TCPSocketInfo>& TCPSocket::GetSocketVector()
{
	return m_socketInfoVec;
}
const char* TCPSocket::GetSocketIP(int index)
{
	TCPSocketInfo* tcpInfo = GetTCPSocketInfo(index);
	if (!tcpInfo)
	{
		return nullptr;
	}

	return tcpInfo->ip;
}
TCPSocketInfo* TCPSocket::GetTCPSocketInfo(int index)
{
	if (index < 0 || index >= m_socketInfoVec.size())
	{
		return nullptr;
	}

	return &m_socketInfoVec[index];
}
bool& TCPSocket::GetRuninged()
{
	return m_running;
}
ServiceType TCPSocket::GetServerType()
{
	return m_ServiceType;
}
event_base* TCPSocket::GetEventBase()
{
	return m_listenerBase;
}
std::vector<std::thread*>& TCPSocket::GetSockeThreadVec()
{
	return m_socketThread;
}

bool TCPSocket::IsConnected(int index)
{
	TCPSocketInfo* tcpInfo = GetTCPSocketInfo(index);
	if (!tcpInfo)
	{
		return false;
	}
	return tcpInfo->isConnect;
}
// 分配索引算法
int TCPSocket::GetSocketIndex()
{
	std::lock_guard<std::mutex> guard(m_mutex);

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

void TCPSocket::SetTcpRcvSndBUF(SOCKFD fd, int rcvBufSize, int sndBufSize)
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
void TCPSocket::SetMaxSingleReadAndWrite(bufferevent* bev, int rcvBufSize, int sndBufSize)
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

int TCPSocket::StreamSocketpair(struct addrinfo* addr_info, SOCKFD sock[2])
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
int TCPSocket::DgramSocketpair(struct addrinfo* addr_info, SOCKFD sock[2])
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
int TCPSocket::Socketpair(int family, int type, int protocol, SOCKFD recv[2])
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
bool TCPSocket::BuffereventWrite(int index, void* data, unsigned int size)
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
bool TCPSocket::SendMsg(int index, const char* pData, size_t size, MsgCmd mainID, int assistID, int handleCode, 
	void* pBufferevent, unsigned int uIdentification/* = 0*/, bool WSPackData/* = true*/)
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

// 发送线程消息处理发送
void TCPSocket::ThreadSendMsg()
{
	CDataLine* pDataLine = GetSendDataLine();
	if (!pDataLine)
	{
		Log(CERR, "send list is null");
		return;
	}
	ListItemData* pListItem = NULL;
	unsigned int uDataKind = 0;
	while (m_running)
	{
		unsigned int bytes = pDataLine->GetData(&pListItem, m_running, uDataKind);
		if (bytes == 0 || pListItem == NULL)
		{
			continue;
		}
		HandleSendMsg(pListItem);
	}

	Log(CINF, "send data thread end");

	return;
}
void TCPSocket::HandleSendMsg(ListItemData* pListItem)
{
	if (!pListItem)
	{
		return;
	}
	if (pListItem->stDataHead.uSize == 0 || pListItem->pData == nullptr)
	{
		return;
	}

	HandleSendData(pListItem);

	SafeDeleteArray(pListItem->pData);
	SafeDelete(pListItem);
}
void TCPSocket::HandleSendData(ListItemData* pListItem)
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

// 接收消息进行解包处理
bool TCPSocket::RecvData(bufferevent* bev, int index)
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
		CloseSocket(index);
		Log(CERR, "消息格式不正确,index=%d", index);
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
			Log(CERR, "消息格式不正确");
			return false;
		}

		int realSize = messageSize - sizeof(NetMessageHead);
		if (realSize < 0)
		{
			// 数据包不够包头
			CloseSocket(index);
			Log(CERR, "数据包不够包头");
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