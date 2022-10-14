#include "../Game/stdafx.h"

CTCPSocketManage::CTCPSocketManage() :
	m_bindIP(""),
	m_running(false),
	m_uMaxSocketSize(0),
	m_uCurSocketSize(0),
	m_uCurSocketIndex(0),
	m_listenerBase(nullptr),
	m_pRecvDataLine(new CDataLine),
	m_pSendDataLine(new CDataLine),
	m_eventBaseCfg(event_config_new())
#ifdef __WebSocket__
	,m_ctx(nullptr)
	, m_cert(nullptr)
#endif // __WebSocket__
{
#if defined(_WIN32)
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
#elif defined(_WIN64)
#elif defined(__linux__)
	if (evthread_use_pthreads() != 0)
	{
		COUT_LOG(LOG_CERROR, "Init thread is err");
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

bool CTCPSocketManage::Init(int maxCount, int port, const char* ip)
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
	m_workBaseVec.clear();
	m_heartBeatSocketSet.clear();

	// ��ʼ�������ڴ�
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
		COUT_LOG(LOG_CERROR, "TCP Could not initialize libevent!");
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
		COUT_LOG(LOG_INFO, "Could not create a listener! ���Ի����˿ڻ����Ե�һ�ᡣ");
		return;
	}

	evconnlistener_set_error_cb(listener, AcceptErrorCB);

	// ��ȡ�����̳߳�����
	int workBaseCount = BaseCfgMgr.GetThreadCnt();
	if (workBaseCount <= 1)
	{
		workBaseCount = 8;
	}

	// ��ʼ�����߳���Ϣ
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
			COUT_LOG(LOG_CERROR, "Socketpair");
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
			COUT_LOG(LOG_CERROR, "TCP Could not initialize libevent!");
			return;
		}

		workInfo.event = event_new(workInfo.base, workInfo.read_fd, EV_READ,
			ThreadLibeventProcess, (void*)&uniqueParam[i]);
		if (!workInfo.event)
		{
			COUT_LOG(LOG_CERROR, "TCP Could not create event!");
			return;
		}

		if (event_add(workInfo.event, nullptr) < 0)
		{
			COUT_LOG(LOG_CERROR, "TCP event_add ERROR");
			return;
		}

		m_workBaseVec.push_back(workInfo);
	}

	std::vector<std::thread> threadVev;
	// ���ٹ����̳߳�
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
			// ��֪��Ϊʲô�˳�ʱ��������
			//event_free(workInfo.event);
		}
	}

	COUT_LOG(LOG_CINFO, "accept thread end");

	return;
}
void CTCPSocketManage::ThreadRSSocket(void* pThreadData)
{
	RecvThreadParam* param = (RecvThreadParam*)pThreadData;
	if (!param)
	{
		COUT_LOG(LOG_CERROR, "thread param is null");
		return;
	}

	// ���ڼ���״̬
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
		COUT_LOG(LOG_CERROR, "######  threadIndex = %d", threadIndex);
		return;
	}

	char buf[sizeof(PlatformSocketInfo) * MAX_POST_CONNECTED_COUNT] = "";

	int realAllSize = recv(readfd, buf, sizeof(buf), 0);
	if (realAllSize < sizeof(PlatformSocketInfo) || realAllSize % sizeof(PlatformSocketInfo) != 0)
	{
		COUT_LOG(LOG_CERROR, "ThreadLibeventProcess error size=%d,sizeof(PlatformSocketInfo)=%lld",
			realAllSize, sizeof(PlatformSocketInfo));
		event_add(pThis->m_workBaseVec[threadIndex].event, nullptr);
		return;
	}

	int countAcceptCount = realAllSize / sizeof(PlatformSocketInfo);
	for (int i = 0; i < countAcceptCount; i++)
	{
		PlatformSocketInfo* pTCPSocketInfo = (PlatformSocketInfo*)(buf + i * sizeof(PlatformSocketInfo));

		// ��������
		pThis->AddTCPSocketInfo(threadIndex, pTCPSocketInfo);
	}

	event_add(pThis->m_workBaseVec[threadIndex].event, nullptr);
}

void CTCPSocketManage::AddTCPSocketInfo(int threadIndex, PlatformSocketInfo* pTCPSocketInfo)
{
	struct event_base* base = m_workBaseVec[threadIndex].base;
	struct bufferevent* bev = nullptr;
	SOCKFD fd = pTCPSocketInfo->acceptFd;

	// ���������㷨
	int index = GetSocketIndex();
	if (index < 0)
	{
		COUT_LOG(LOG_CERROR, "��������ʧ�ܣ�����fd=%d,ip=%s", fd, pTCPSocketInfo->ip);
		closesocket(fd);
		return;
	}

	bev = bufferevent_socket_new(base, fd, /*BEV_OPT_CLOSE_ON_FREE | */BEV_OPT_THREADSAFE);
	if (!bev)
	{
		COUT_LOG(LOG_CERROR, "Error constructing bufferevent!,fd=%d,ip=%s", fd, pTCPSocketInfo->ip);
		closesocket(fd);
		return;
	}

	// ����Ӧ�ò��շ����ݰ������δ�С
	SetMaxSingleReadAndWrite(bev, SOCKET_RECV_BUF_SIZE, SOCKET_SEND_BUF_SIZE);

	// ���ɻص���������������bufferevent_freeҪ�ͷ��ڴ棬�����ڴ�й¶
	RecvThreadParam* pRecvThreadParam = new RecvThreadParam;
	pRecvThreadParam->pThis = this;
	pRecvThreadParam->index = index;

	// ����¼��������úûص�����
	bufferevent_setcb(bev, ReadCB, nullptr, EventCB, (void*)pRecvThreadParam);
	if (bufferevent_enable(bev, EV_READ | EV_ET) < 0)
	{
		COUT_LOG(LOG_CERROR, "add event fail!!!,fd=%d,ip=%s", fd, pTCPSocketInfo->ip);
		closesocket(fd);
		bufferevent_free(bev);
		SafeDelete(pRecvThreadParam);
		return;
	}

	// ���ö���ʱ���������������ط���������Ҫ
	if (m_iServiceType == ServiceType::SERVICE_TYPE_LOGIC || 
		m_iServiceType == ServiceType::SERVICE_TYPE_LOGIC_WS)
	{
		timeval tvRead;
		tvRead.tv_sec = CHECK_HEAETBEAT_SECS * KEEP_ACTIVE_HEARTBEAT_COUNT;
		tvRead.tv_usec = 0;
		bufferevent_set_timeouts(bev, &tvRead, nullptr);
	}

	// ������Ϣ
	TCPSocketInfo tcpInfo;
	memcpy(tcpInfo.ip, pTCPSocketInfo->ip, sizeof(tcpInfo.ip));
	tcpInfo.acceptFd = pTCPSocketInfo->acceptFd;
	tcpInfo.acceptMsgTime = pTCPSocketInfo->acceptMsgTime;
	tcpInfo.bev = bev;
	tcpInfo.isConnect = true;
	tcpInfo.port = pTCPSocketInfo->port;
	tcpInfo.link = DUtil->CreateUserId() + (uint64_t)Util::GetRandNum();
	if (!tcpInfo.lock)
	{
		tcpInfo.lock = new std::mutex;
	}
	tcpInfo.bHandleAccptMsg = false;

	m_ConditionVariable.GetMutex().lock();	//����
	if (m_socketInfoVec[index].isConnect)
	{
		m_ConditionVariable.GetMutex().unlock(); //����
		COUT_LOG(LOG_CERROR, "��������ʧ��,fd=%d,ip=%s", fd, pTCPSocketInfo->ip);
		closesocket(fd);
		bufferevent_free(bev);
		SafeDelete(pRecvThreadParam);
		return;
	}
	m_socketInfoVec[index] = tcpInfo;
	m_heartBeatSocketSet.insert((unsigned int)index);
	m_uCurSocketSize++;
	m_ConditionVariable.GetMutex().unlock(); //����

	if (m_iServiceType != ServiceType::SERVICE_TYPE_LOGIC_WS)
	{
		// TCP������ ��֤�ͻ���
		Netmsg msg; msg << tcpInfo.link;
		SendMsg(index, msg.str().c_str(), msg.str().size(), MsgCmd::MsgCmd_Testlink, 0, 0, tcpInfo.bev);
	}
}

void CTCPSocketManage::ListenerCB(evconnlistener* listener, evutil_socket_t fd, sockaddr* sa, int socklen, void* data)
{
	CTCPSocketManage* pThis = (CTCPSocketManage*)data;

	// ��ȡ������Ϣ
	struct sockaddr_in* addrClient = (struct sockaddr_in*)sa;
	PlatformSocketInfo tcpInfo;
	tcpInfo.acceptMsgTime = time(nullptr);
	strcpy(tcpInfo.ip, inet_ntoa(addrClient->sin_addr));
	tcpInfo.port = ntohs(addrClient->sin_port);
	tcpInfo.acceptFd = fd;	//������accept�����׽��������Ϳͻ���ͨ��

	if (pThis->GetCurSocketSize() >= pThis->m_uMaxSocketSize)
	{
		COUT_LOG(LOG_CERROR, "�������Ѿ�����fd=%d [ip:%s %d][������%u/%u]", fd,
			tcpInfo.ip, tcpInfo.port, pThis->GetCurSocketSize(), pThis->m_uMaxSocketSize);

		// ����ʧ��
		NetMessageHead netHead;

		netHead.uMainID = 100;
		netHead.uAssistantID = 3;
		netHead.uHandleCode = ERROR_SERVICE_FULL;//��������������
		netHead.uMessageSize = sizeof(NetMessageHead);

		sendto(fd, (char*)&netHead, sizeof(NetMessageHead), 0, (sockaddr*)&sa, sizeof(sockaddr_in));

		closesocket(fd);

		return;
	}

	// ���õײ��շ�������
	SetTcpRcvSndBUF(fd, SOCKET_RECV_BUF_SIZE, SOCKET_SEND_BUF_SIZE);

	// memcached���̸߳��ؾ����㷨
	static int lastThreadIndex = 0;
	lastThreadIndex = lastThreadIndex % pThis->m_workBaseVec.size();

	// Ͷ�ݵ������߳�
	if (send(pThis->m_workBaseVec[lastThreadIndex].write_fd,
		(const char*)(&tcpInfo), sizeof(tcpInfo), 0) < sizeof(tcpInfo))
	{
		COUT_LOG(LOG_CERROR, "Ͷ��������Ϣʧ��,fd=%d", fd);
	}

	lastThreadIndex++;
}
void CTCPSocketManage::ReadCB(bufferevent* bev, void* data)
{
	RecvThreadParam* param = (RecvThreadParam*)data;
	CTCPSocketManage* pThis = param->pThis;
	int index = param->index;

	// �������ݣ���ͷ����
	pThis->RecvData(bev, index);
}
void CTCPSocketManage::EventCB(bufferevent* bev, short events, void* data)
{
	RecvThreadParam* param = (RecvThreadParam*)data;
	CTCPSocketManage* pThis = param->pThis;
	int index = param->index;

	if (events & BEV_EVENT_EOF)
	{
		// ��������
	}
	else if (events & BEV_EVENT_ERROR)
	{
		// windows��������
	}
	else if (events & BEV_EVENT_TIMEOUT) // ��ʱ��û���յ����ͻ��˷����������ݣ���ȡ���ݳ�ʱ
	{
		COUT_LOG(LOG_INFO, "�������� index=%d fd=%d", index, pThis->m_socketInfoVec[index].acceptFd);
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

// ��������
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
	auto& tcpInfo = m_socketInfoVec[index];
	Util::Decrypt((char*)str.c_str(), str.size());
	if (std::to_string(tcpInfo.link) != str)
	{
		return false;
	}
	
	tcpInfo.link = (uint64_t)MsgCmd::MsgCmd_Testlink;

	COUT_LOG(LOG_CINFO, "TCP connect [ip=%s port=%d index=%d fd=%d bufferevent=%p]",
		tcpInfo.ip, tcpInfo.port, index, tcpInfo.acceptFd, tcpInfo.bev);

	return true;
}

// ������Ϣ�ɷ�
bool CTCPSocketManage::DispatchPacket(void* pBufferevent, int index, NetMessageHead* pHead, void* pData, int size, SocketType socketType/* = SocketType::SOCKET_TYPE_TCP*/)
{
	if (!pBufferevent || !pHead)
	{
		return false;
	}
	if (pHead->uMainID == (unsigned int)MsgCmd::MsgCmd_HeartBeat) //������
	{
		return true;
	}
	if (pHead->uMainID == (unsigned int)MsgCmd::MsgCmd_Testlink) //�������Ӱ�
	{
		if (!VerifyConnection(index, (char*)pData))
		{
			RemoveTCPSocketStatus(index);
		}
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

//����رմ���
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
	if (index < 0 || index >= m_socketInfoVec.size())
	{
		COUT_LOG(LOG_CERROR, "index=%d ������Χ", index);
		return;
	}

	unsigned long uAccessIP = 0;

	// ����
	m_ConditionVariable.GetMutex().lock();

	auto& tcpInfo = m_socketInfoVec[index];

	// �ظ�����
	if (!tcpInfo.isConnect)
	{
		return;
	}

	// �����û�з����ڴ棬�ͷ���
	if (!tcpInfo.lock)
	{
		tcpInfo.lock = new std::mutex;
	}

	uAccessIP = inet_addr(tcpInfo.ip);
	m_uCurSocketSize--;
	m_heartBeatSocketSet.erase((unsigned int)index);

	// �ͷŲ����ڴ�
	RecvThreadParam* pRecvThreadParam = (RecvThreadParam*)0x01;
	bufferevent_getcb(tcpInfo.bev, nullptr, nullptr, nullptr, (void**)&pRecvThreadParam);
	if (pRecvThreadParam)
	{
		SafeDelete(pRecvThreadParam);
	}

	//// ��������������FIN��
	//if (!isClientAutoClose)
	//{
	//	closesocket(tcpInfo.acceptFd);
	//}

	// �ͷ����߳���ص���
	tcpInfo.lock->lock();

	tcpInfo.isConnect = false;
	tcpInfo.bHandleAccptMsg = false;
	bufferevent_free(tcpInfo.bev);
	tcpInfo.bev = nullptr;

	// ���������߳�
	tcpInfo.lock->unlock();

	// �������߳�
	m_ConditionVariable.GetMutex().unlock();

	// ���û������BEV_OPT_CLOSE_ON_FREE ѡ���ر�socket
	closesocket(tcpInfo.acceptFd);

	OnSocketCloseEvent(uAccessIP, index, (unsigned int)tcpInfo.acceptMsgTime);

	// �����¼�ڴ�
	DPPC->GetLoginSys().DelLoginInMap(index);

	COUT_LOG(LOG_CINFO, "TCP close [ip=%s port=%d index=%d fd=%d isClientAutoClose:%d acceptTime=%lld]",
		tcpInfo.ip, tcpInfo.port, index, tcpInfo.acceptFd, isClientAutoClose, tcpInfo.acceptMsgTime);
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
bool& CTCPSocketManage::GetRuninged()
{
	return m_running;
}
ServiceType CTCPSocketManage::GetServerType()
{
	return m_iServiceType;
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
	if (index < 0 || index >= m_socketInfoVec.size())
	{
		return false;
	}

	return m_socketInfoVec[index].isConnect;
}
// ���������㷨
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
		COUT_LOG(LOG_CERROR, "bufferevent_set_max_single_read fail,bev=%p", bev);
	}

	/*if (bufferevent_set_max_single_write(bev, sndBufSize) < 0)
	{
		COUT_LOG(LOG_CERROR, "bufferevent_set_max_single_write fail,fd=%d", fd);
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
	listener = socket(addr_info->ai_family, addr_info->ai_socktype, addr_info->ai_protocol); //����������socket�����а󶨼�����
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

	client = socket(addr_info->ai_family, addr_info->ai_socktype, addr_info->ai_protocol); //�����ͻ���socket�������ӷ�����

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

// ��Ϣ����
bool CTCPSocketManage::BuffereventWrite(int index, void* data, unsigned int size)
{
	//��������
	TCPSocketInfo& tcpInfo = m_socketInfoVec[index];
	if (!tcpInfo.lock)
	{
		return false;
	}
	{
		std::lock_guard<std::mutex> guard(*tcpInfo.lock);
		if (tcpInfo.isConnect && tcpInfo.bev)
		{
			if (bufferevent_write(tcpInfo.bev, data, size) < 0)
			{
				COUT_LOG(LOG_CERROR, "��������ʧ�ܣ�index=%d socketfd=%d bev=%p,", index, tcpInfo.acceptFd, tcpInfo.bev);
			}
		}
	}

	return true;
}
bool CTCPSocketManage::BuffereventWrite(int index, std::string& data)
{
	//��������
	TCPSocketInfo& tcpInfo = m_socketInfoVec[index];
	if (!tcpInfo.lock)
	{
		return false;
	}
	{
		std::lock_guard<std::mutex> guard(*tcpInfo.lock);
		if (tcpInfo.isConnect && tcpInfo.bev)
		{
			if (bufferevent_write(tcpInfo.bev, data.c_str(), data.size()) < 0)
			{
				COUT_LOG(LOG_CERROR, "��������ʧ�ܣ�index=%d socketfd=%d bev=%p,", index, tcpInfo.acceptFd, tcpInfo.bev);
			}
		}
	}

	return true;
}
bool CTCPSocketManage::SendMsg(int index, const char* pData, size_t size, MsgCmd mainID, int assistID, int handleCode, 
	void* pBufferevent, unsigned int uIdentification/* = 0*/, bool WSPackData/* = true*/)
{
	if (m_iServiceType == ServiceType::SERVICE_TYPE_LOGIC_WS)
	{
#ifdef __WebSocket__
		// websocket������
		return WSSendWSLogicMsg(index, pData, size, mainID, assistID, handleCode, pBufferevent, uIdentification, WSPackData);
#endif // __WebSocket__
	}
	else
	{
		// TCP������
		return SendLogicMsg(index, pData, size, mainID, assistID, handleCode, pBufferevent, uIdentification);
	}

	return true;
}
bool CTCPSocketManage::SendLogicMsg(int index, const char* pData, size_t size, MsgCmd mainID, int assistID, int handleCode, 
	void* pBufferevent, unsigned int uIdentification/* = 0*/)
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

	// ����һ������
	std::unique_ptr<char[]> SendBuf(new char[sizeof(SendDataLineHead) + sizeof(NetMessageHead) + size]);

	// ƴ�Ӱ�ͷ
	NetMessageHead* pHead = reinterpret_cast<NetMessageHead*>((char*)SendBuf.get() + sizeof(SendDataLineHead));
	pHead->uMainID = (unsigned int)mainID;
	pHead->uAssistantID = assistID;
	pHead->uMessageSize = (unsigned int)(sizeof(NetMessageHead) + size);
	pHead->uHandleCode = handleCode;
	pHead->uIdentification = uIdentification;

	// ����
	if (pData && size > 0)
	{
		memcpy(SendBuf.get() + sizeof(SendDataLineHead) + sizeof(NetMessageHead), pData, size);
	}

	// Ͷ�ݵ����Ͷ���
	if (m_pSendDataLine)
	{
		SendDataLineHead* pLineHead = reinterpret_cast<SendDataLineHead*>(SendBuf.get());
		pLineHead->dataLineHead.uSize = pHead->uMessageSize;
		pLineHead->socketIndex = index;
		pLineHead->pBufferevent = pBufferevent;

		unsigned int addBytes = m_pSendDataLine->AddData(pLineHead, sizeof(SendDataLineHead) + pHead->uMessageSize);

		if (addBytes == 0)
		{
			COUT_LOG(LOG_CERROR, "Ͷ����Ϣʧ��,mainID=%d,assistID=%d", mainID, assistID);
			return false;
		}
	}

	return true;
}
#ifdef __WebSocket__
bool CTCPSocketManage::WSSendWSLogicMsg(int index, const char* pData, size_t size, MsgCmd mainID, int assistID, int handleCode, 
	void* pBufferevent, unsigned int uIdentification/* = 0*/, bool PackData/* = true*/)
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
	// ����װ����
	if (!PackData)
	{
		if (size < 0 || size > MAX_TEMP_SENDBUF_SIZE)
		{
			COUT_LOG(LOG_CERROR, "invalid message size size=%lld", size);
			return false;
		}
		// ����һ������
		std::unique_ptr<char[]> SendBuf(new char[sizeof(SendDataLineHead) + size]);

		// ����
		if (pData && size > 0)
		{
			memcpy(SendBuf.get() + sizeof(SendDataLineHead), pData, size);
		}

		// Ͷ�ݵ����Ͷ���
		if (m_pSendDataLine)
		{
			SendDataLineHead* pLineHead = reinterpret_cast<SendDataLineHead*>(SendBuf.get());
			pLineHead->dataLineHead.uSize = (unsigned int)size;
			pLineHead->socketIndex = index;
			pLineHead->pBufferevent = pBufferevent;

			unsigned int addBytes = m_pSendDataLine->AddData(pLineHead, sizeof(SendDataLineHead) + (unsigned int)size);

			if (addBytes == 0)
			{
				COUT_LOG(LOG_CERROR, "Ͷ����Ϣʧ��,mainID=%d,assistID=%d", mainID, assistID);
				return false;
			}
		}
	}
	// ��װ����
	else
	{
		if (size < 0 || size > MAX_TEMP_SENDBUF_SIZE - sizeof(NetMessageHead))
		{
			COUT_LOG(LOG_CERROR, "invalid message size size=%lld", size);
			return false;
		}

		// ����һ������
		std::unique_ptr<char[]> SendBuf(new char[sizeof(SendDataLineHead) + sizeof(NetMessageHead) + size]);

		// ƴ�Ӱ�ͷ
		NetMessageHead* pHead = reinterpret_cast<NetMessageHead*>((char*)SendBuf.get() + sizeof(SendDataLineHead));
		pHead->uMainID = (unsigned int)mainID;
		pHead->uAssistantID = assistID;
		pHead->uMessageSize = (unsigned int)(sizeof(NetMessageHead) + size);
		pHead->uHandleCode = handleCode;
		pHead->uIdentification = uIdentification;

		// ����
		if (pData && size > 0)
		{
			memcpy(SendBuf.get() + sizeof(SendDataLineHead) + sizeof(NetMessageHead), pData, size);
		}

		// Ͷ�ݵ����Ͷ���
		if (m_pSendDataLine)
		{
			SendDataLineHead* pLineHead = reinterpret_cast<SendDataLineHead*>(SendBuf.get());
			pLineHead->dataLineHead.uSize = pHead->uMessageSize;
			pLineHead->socketIndex = index;
			pLineHead->pBufferevent = pBufferevent;

			unsigned int addBytes = m_pSendDataLine->AddData(pLineHead, sizeof(SendDataLineHead) + pHead->uMessageSize);

			if (addBytes == 0)
			{
				COUT_LOG(LOG_CERROR, "Ͷ����Ϣʧ��,mainID=%d,assistID=%d", mainID, assistID);
				return false;
			}
		}
	}

	return true;
}
#endif // __WebSocket__

// �����߳���Ϣ������
void CTCPSocketManage::ThreadSendMsg()
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
			HandleSendMsg(dataList.front());
			dataList.pop_front();
		}
	}

	COUT_LOG(LOG_CINFO, "send data thread end");

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
	if (m_iServiceType == ServiceType::SERVICE_TYPE_LOGIC_WS)
	{
#ifdef __WebSocket__
		// websocket������
		WSHandleSendWSData(pListItem);
#endif // __WebSocket__
	}
	else
	{
		// TCP������
		HandleSendData(pListItem);
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

	if (index < 0 || index >= m_socketInfoVec.size())
	{
		COUT_LOG(LOG_CERROR, "��������ʧ�ܣ�index=%d ������Χ", index);
		return;
	}
	if (!BuffereventWrite(index, pData, size))
	{
		return;
	}
}
#ifdef __WebSocket__
void CTCPSocketManage::WSHandleSendWSData(ListItemData* pListItem)
{
	SendDataLineHead* pSocketSend = reinterpret_cast<SendDataLineHead*>(pListItem->pData);
	unsigned int size = pSocketSend->dataLineHead.uSize;
	int index = pSocketSend->socketIndex;
	void* pData = pListItem->pData + sizeof(SendDataLineHead);

	if (index < 0 || index >= m_socketInfoVec.size())
	{
		COUT_LOG(LOG_CERROR, "��������ʧ�ܣ�index=%d ������Χ", index);
		return;
	}
	TCPSocketInfo& tcpInfo = m_socketInfoVec[index];
	// websocket ����
	if (!tcpInfo.bHandleAccptMsg)
	{
		//��������
		if (!tcpInfo.lock)
		{
			return;
		}
		{
			std::lock_guard<std::mutex> guard(*tcpInfo.lock);
			if (tcpInfo.isConnect && tcpInfo.bev)
			{
				if (bufferevent_write(tcpInfo.bev, pData, size) < 0)
				{
					COUT_LOG(LOG_CERROR, "��������ʧ�ܣ�index=%d socketfd=%d bev=%p,", index, tcpInfo.acceptFd, tcpInfo.bev);
				}
				// ����Ѿ���������
				tcpInfo.bHandleAccptMsg = true;
			}
		}
		if (tcpInfo.bHandleAccptMsg)
		{
			// websocket������ ���ֳɹ� ��֤�ͻ���
			Netmsg msg; msg << tcpInfo.link;
			SendMsg(index, msg.str().c_str(), msg.str().size(), MsgCmd::MsgCmd_Testlink, 0, 0, tcpInfo.bev);
		}
		return;
	}
	// websocket ���ֳɹ�
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
			COUT_LOG(LOG_CERROR, "web socket send to big size=%d", size);
			return;
		}

		// ����һ������
		std::unique_ptr<char[]> recvBuf(new char[frame_size]);
		char* frame = recvBuf.get();

		// ���websocket��ͷ
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
		// Ӧ�ò����
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
#endif // __WebSocket__

// ������Ϣ���н������
bool CTCPSocketManage::RecvData(bufferevent* bev, int index)
{
	if (m_iServiceType == ServiceType::SERVICE_TYPE_LOGIC_WS)
	{
#ifdef __WebSocket__
		// websocket������
		if (!WSRecvWSLogicData(bev, index))
		{
			return false;
		}
#endif // __WebSocket__
	}
	else
	{
		// TCP������
		if (!RecvLogicData(bev, index))
		{
			return false;
		}
	}

	return true;
}
bool CTCPSocketManage::RecvLogicData(bufferevent* bev, int index)
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

	// ʣ�ദ������
	size_t handleRemainSize = realAllSize;

	// �����ͷ
	NetMessageHead* pNetHead = (NetMessageHead*)recvBuf.get();

	// �����ж�
	if (handleRemainSize >= sizeof(NetMessageHead) && pNetHead->uMessageSize > SOCKET_RECV_BUF_SIZE)
	{
		// ��Ϣ��ʽ����ȷ
		CloseSocket(index);
		COUT_LOG(LOG_CERROR, "��Ϣ��ʽ����ȷ,index=%d", index);
		return false;
	}

	// ճ������
	while (handleRemainSize >= sizeof(NetMessageHead) && handleRemainSize >= pNetHead->uMessageSize)
	{
		unsigned int messageSize = pNetHead->uMessageSize;
		if (messageSize > MAX_TEMP_SENDBUF_SIZE)
		{
			// ��Ϣ��ʽ����ȷ
			CloseSocket(index);
			COUT_LOG(LOG_CERROR, "��Ϣ��ʽ����ȷ");
			return false;
		}

		int realSize = messageSize - sizeof(NetMessageHead);
		if (realSize < 0)
		{
			// ���ݰ�������ͷ
			CloseSocket(index);
			COUT_LOG(LOG_CERROR, "���ݰ�������ͷ");
			return false;
		}

		void* pData = nullptr;
		if (realSize > 0)
		{
			// û���ݾ�Ϊnullptr
			pData = (void*)(recvBuf.get() + realAllSize - handleRemainSize + sizeof(NetMessageHead));
		}

		// �ɷ�����
		DispatchPacket(bev, index, pNetHead, pData, realSize, SocketType::SOCKET_TYPE_TCP);

		handleRemainSize -= messageSize;

		pNetHead = (NetMessageHead*)(recvBuf.get() + realAllSize - handleRemainSize);
	}

	evbuffer_drain(input, realAllSize - handleRemainSize);
	return true;
}
#ifdef __WebSocket__
bool CTCPSocketManage::WSRecvWSLogicData(bufferevent* bev, int index)
{
	if (bev == nullptr)
	{
		COUT_LOG(LOG_CERROR, "RecvData error bev == nullptr");
		return false;
	}
	if (!m_socketInfoVec[index].bHandleAccptMsg)
	{
		return WSHandShark(bev, index);
	}

	struct evbuffer* input = bufferevent_get_input(bev);

	size_t maxSingleRead = Min_(evbuffer_get_length(input), SOCKET_RECV_BUF_SIZE);

	std::unique_ptr<char[]> recvBuf(new char[maxSingleRead]);

	size_t realAllSize = evbuffer_copyout(input, recvBuf.get(), maxSingleRead);
	if (realAllSize <= 0)
	{
		return false;
	}

	// ʣ�ദ������
	size_t handleRemainSize = realAllSize;

	// ����һ����ͷ
	if (handleRemainSize < MIN_WEBSOCKET_HEAD_SIZE + sizeof(NetMessageHead))
	{
		return true;
	}

	char* pBuffer = recvBuf.get();
	WebSocketMsg wbmsg;

	// ճ������ÿһ��ѭ������һ�����ݰ�
	while (true)
	{
		// ����һ����ͷ
		if (handleRemainSize < MIN_WEBSOCKET_HEAD_SIZE + sizeof(NetMessageHead))
		{
			break;
		}

		// ����websocket��ͷ
		int pos = 0;
		wbmsg.Init();
		WSFetchFin(pBuffer, pos, wbmsg);
		WSFetchOpcode(pBuffer, pos, wbmsg);
		WSFetchMask(pBuffer, pos, wbmsg);
		WSFetchPayloadLength(pBuffer, pos, wbmsg);
		WSFetchMaskingKey(pBuffer, pos, wbmsg);

		if (wbmsg.dataLength > SOCKET_RECV_BUF_SIZE)
		{
			// ��Ϣ��ʽ����ȷ
			CloseSocket(index);
			COUT_LOG(LOG_CERROR, "��Ϣ��ʽ����ȷ,index=%d,maxsize=%u,wbmsg.dataLength=%u",
				index, SOCKET_RECV_BUF_SIZE, wbmsg.dataLength);
			return false;
		}

		// ���ݲ���һ�������İ�������������
		if (handleRemainSize < wbmsg.dataLength)
		{
			break;
		}

		WSFetchPayload(pBuffer, pos, wbmsg);
		//WSFetchPrint(wbmsg);

		// ����Ӧ�ò��ͷ
		NetMessageHead* pNetHead = (NetMessageHead*)wbmsg.payload;

		// Э���ʽ�ж�
		if (pNetHead->uMessageSize > SOCKET_RECV_BUF_SIZE || pNetHead->uMessageSize != wbmsg.payloadLength)
		{
			// ��Ϣ��ʽ����ȷ
			CloseSocket(index);
			COUT_LOG(LOG_CERROR, "��Ϣ��ʽ����ȷ,index=%d,pNetHead->uMessageSize=%u,wbmsg.payloadLength=%u",
				index, pNetHead->uMessageSize, wbmsg.payloadLength);
			return false;
		}

		unsigned int messageSize = pNetHead->uMessageSize;
		if (messageSize > MAX_TEMP_SENDBUF_SIZE)
		{
			// ��Ϣ��ʽ����ȷ
			CloseSocket(index);
			COUT_LOG(LOG_CERROR, "��Ϣ��ʽ����ȷ,index=%d,messageSize=%u", index, messageSize);
			return false;
		}

		int realSize = messageSize - sizeof(NetMessageHead);
		if (realSize < 0)
		{
			// ���ݰ�������ͷ
			CloseSocket(index);
			COUT_LOG(LOG_CERROR, "���ݰ�������ͷ,index=%d,realSize=%d", index, realSize);
			return false;
		}

		void* pData = NULL;
		if (realSize > 0)
		{
			// û���ݾ�ΪNULL
			pData = (void*)(wbmsg.payload + sizeof(NetMessageHead));
		}

		// �ɷ�����
		DispatchPacket(bev, index, pNetHead, pData, realSize, SocketType::SOCKET_TYPE_WEBSOCKET);

		handleRemainSize -= pos;

		pBuffer = pBuffer + pos;
	}

	// ɾ��buffer����
	evbuffer_drain(input, realAllSize - handleRemainSize);

	return true;
}
#endif // __WebSocket__

#ifdef __WebSocket__
bool CTCPSocketManage::WSOpensslCert(std::string path)
{
	FILE* fp = fopen(path.c_str(), "r");
	if (!fp) 
	{
		COUT_LOG(LOG_CERROR, "unable to open: %s", path.c_str());
		return false;
	}

	X509* cert = PEM_read_X509(fp, NULL, NULL, NULL);
	if (!cert) 
	{
		COUT_LOG(LOG_CERROR, "unable to parse certificate in: %s", path.c_str());
		fclose(fp);
		return false;
	}

	// any additional processing would go here..

	X509_free(cert);
	fclose(fp);
	return true;
}
bool CTCPSocketManage::WSOpensslInit()
{
	SSL_library_init();//��ʼ����
	OpenSSL_add_all_algorithms();
	SSL_load_error_strings();//���ش�����Ϣ
	m_ctx = SSL_CTX_new(SSLv23_method());//SSLv23_server_method or SSLv23_client_method 
	if (nullptr == m_ctx)
	{
		COUT_LOG(LOG_CERROR, "SSL_CTX_new error");
		return false;
	}
	SSL_CTX_set_verify(m_ctx, SSL_VERIFY_NONE, nullptr);//ȡ����֤ǰ��֤��
	// �������θ�֤��
	if (SSL_CTX_load_verify_locations(m_ctx, "chain.crt", NULL) <= 0)
	{
		COUT_LOG(LOG_CERROR, "SSL_CTX_load_verify_locations kill myself,%s", ERR_error_string(ERR_get_error(), NULL));
		return false;
	}
	if (1 != SSL_CTX_use_certificate_file(m_ctx, m_cert.data(), SSL_FILETYPE_PEM))//����֤��
	{
		COUT_LOG(LOG_CERROR, "SSL_CTX_use_certificate_file error");
		return false;
	}
	if (1 != SSL_CTX_use_certificate_chain_file(m_ctx, m_cert.data()))//����֤����
	{
		COUT_LOG(LOG_CERROR, "SSL_CTX_use_certificate_chain_file error");
		return false;
	}
	if (1 != SSL_CTX_use_PrivateKey_file(m_ctx, m_key.data(), SSL_FILETYPE_PEM))//������Կ
	{
		COUT_LOG(LOG_CERROR, "SSL_CTX_use_PrivateKey_file error");
		return false;
	}
	if (1 != SSL_CTX_check_private_key(m_ctx))//��֤��Կ
	{
		COUT_LOG(LOG_CERROR, "SSL_CTX_check_private_key error");
		return false;
	}

	return true;
}
// ����openssl����
bool CTCPSocketManage::WSOpensslHandShark(bufferevent* bev, int index)
{
	evutil_socket_t fd = bufferevent_getfd(bev);
	return true;
}
#endif // __WebSocket__

#ifdef __WebSocket__
bool CTCPSocketManage::WSHandShark(bufferevent* bev, int index)
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
	std::map<std::string, std::string> headMap;

	std::getline(s, request);
	if (request[request.size() - 1] == '\r')
	{
		request.erase(request.end() - 1);
	}
	else
	{
		CloseSocket(index);
		COUT_LOG(LOG_CINFO, "��Ϣ��ʽ����ȷ,request=%s", request.c_str());
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

	TCPSocketInfo& tcpInfo = m_socketInfoVec[index];
	//��������
	SendMsg(index, requestSendStr.c_str(), requestSendStr.size(), 
		MsgCmd::MsgCmd_HandShark, 0, 0, tcpInfo.bev, 0, tcpInfo.bHandleAccptMsg);
	
	return true;
}
#endif // __WebSocket__

#ifdef __WebSocket__
int CTCPSocketManage::WSFetchFin(char* msg, int& pos, WebSocketMsg& wbmsg)
{
	wbmsg.fin = (unsigned char)msg[pos] >> 7;
	return 0;
}
int CTCPSocketManage::WSFetchOpcode(char* msg, int& pos, WebSocketMsg& wbmsg)
{
	wbmsg.opcode = msg[pos] & 0x0f;
	pos++;
	return 0;
}
int CTCPSocketManage::WSFetchMask(char* msg, int& pos, WebSocketMsg& wbmsg)
{
	wbmsg.mask = (unsigned char)msg[pos] >> 7;
	return 0;
}
int CTCPSocketManage::WSFetchMaskingKey(char* msg, int& pos, WebSocketMsg& wbmsg)
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
int CTCPSocketManage::WSFetchPayloadLength(char* msg, int& pos, WebSocketMsg& wbmsg)
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
int CTCPSocketManage::WSFetchPayload(char* msg, int& pos, WebSocketMsg& wbmsg)
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
void CTCPSocketManage::WSFetchPrint(const WebSocketMsg& wbmsg)
{
	printf("WEBSOCKET PROTOCOL FIN: %d OPCODE: %d MASK: %d DATALEN:%u PAYLOADLEN: %u\n",
		wbmsg.fin, wbmsg.opcode, wbmsg.mask, wbmsg.dataLength, wbmsg.payloadLength);
}
#endif // __WebSocket__

