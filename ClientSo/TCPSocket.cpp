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
	m_ServerIndex(-1),
	m_IsConnect(false)
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

	// ��ʼ�������ڴ�
	unsigned int socketInfoVecSize = m_uMaxSocketSize * 2;
	m_socketInfoVec.resize((size_t)socketInfoVecSize);

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

	int timerCnt = G_CfgMgr->GetCBaseCfgMgr().GetTimerCnt();
	for (int i = 0; i < timerCnt; i++)
	{
		G_PlayerPrepClient->GetCServerTimer()[i].SetTimerRun(false);
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
	// ��ȡ�����̳߳�����
	int workBaseCount = G_CfgMgr->GetCBaseCfgMgr().GetThreadCnt();
	if (workBaseCount <= 1)
	{
		workBaseCount = 2;
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
	// ���ٹ����̳߳�
	for (int i = 0; i < workBaseCount; i++)
	{
		threadVev.push_back(std::thread(ThreadRSSocket, (void*)&uniqueParam[i]));
	}

	for (int i = 0; i < workBaseCount; i++)
	{
		threadVev[i].join();

		WorkThreadInfo& workInfo = m_workBaseVec[i];

		closesocket(workInfo.read_fd);
		closesocket(workInfo.write_fd);
	}

	Log(CINF, "accept thread end");

	return;
}
bool CTCPSocketManage::WaitConnect(int threadIndex)
{
	// ��ȡ�����̳߳�����
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
SOCKFD CTCPSocketManage::GetNewSocket()
{
	SOCKFD sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
	{
		Log(CINF, "����socketʧ��");
		return false;
	}

	return sock;
}
bool CTCPSocketManage::ConnectServer()
{
	int workBaseCount = G_CfgMgr->GetCBaseCfgMgr().GetThreadCnt();
	if (workBaseCount <= 1)
	{
		workBaseCount = 2;
	}
	SOCKFD sock = 0;
	for (int i = 0; i < workBaseCount; i++)
	{
		if (!WaitConnect(i))
		{
			return false;
		}
	}
	sock = GetNewSocket();
	while (true)
	{
		if (ConnectLogicServer(sock))
		{
			break;
		}
		std::this_thread::sleep_for(std::chrono::seconds(5));
	}
	return true;
}
void CTCPSocketManage::ServerSocketInfo(PlatformSocketInfo* tcpInfo)
{
	// ���õײ��շ�������
	SetTcpRcvSndBUF(tcpInfo->acceptFd, SOCKET_RECV_BUF_SIZE, SOCKET_SEND_BUF_SIZE);

	static int lastThreadIndex = 0;
	lastThreadIndex = lastThreadIndex % m_workBaseVec.size();
	// Ͷ�ݵ������߳�
	if (send(m_workBaseVec[lastThreadIndex].write_fd, (const char*)(tcpInfo), 
		sizeof(PlatformSocketInfo), 0) < sizeof(PlatformSocketInfo))
	{
		Log(CERR, "Ͷ��������Ϣʧ��,fd=%d", tcpInfo->acceptFd);
	}
}
bool CTCPSocketManage::ConnectLogicServer(SOCKFD& sock)
{
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(m_ClientInfo.port);
	sin.sin_addr.s_addr = inet_addr(m_ClientInfo.ip.c_str());

	if (connect(sock, (sockaddr*)&sin, sizeof(sockaddr_in)) < 0)
	{
		Log(CINF, "�����߼�������ʧ��");
		return false;
	}

	// ��ȡ������Ϣ
	PlatformSocketInfo tcpInfo;
	tcpInfo.acceptMsgTime = time(nullptr);
	memcpy(tcpInfo.ip, m_ClientInfo.ip.c_str(), m_ClientInfo.ip.size());
	tcpInfo.port = m_ClientInfo.port;
	tcpInfo.acceptFd = sock;	//������accept�����׽��������Ϳͻ���ͨ��

	m_ServerSock = sock;

	ServerSocketInfo(&tcpInfo);

	Log(CINF, "�����߼��������ɹ� [ip=%s port=%d index=%d fd=%d]",
		tcpInfo.ip, tcpInfo.port, m_ServerIndex, tcpInfo.acceptFd);
	return true;
}
void CTCPSocketManage::ThreadRSSocket(void* pThreadData)
{
	RecvThreadParam* param = (RecvThreadParam*)pThreadData;
	if (!param)
	{
		Log(CERR, "thread param is null");
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
#ifdef __WebSocketOpenssl__
	SSL* ssl = nullptr;
#endif

	// ���������㷨
	int index = GetSocketIndex();
	if (index < 0)
	{
		Log(CERR, "��������ʧ�ܣ�����fd=%d,ip=%s", fd, pTCPSocketInfo->ip);
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
		Log(CERR, "add event fail!!!,fd=%d,ip=%s", fd, pTCPSocketInfo->ip);
		closesocket(fd);
		bufferevent_free(bev);
		SafeDelete(pRecvThreadParam);
		return;
	}

	// ���ö���ʱ���������������ط���������Ҫ
	if (m_ServiceType == ServiceType::SERVICE_TYPE_LOGIC ||
		m_ServiceType == ServiceType::SERVICE_TYPE_LOGIC_WS ||
		m_ServiceType == ServiceType::SERVICE_TYPE_LOGIC_WSS)
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
	if (!tcpInfo.lock)
	{
		tcpInfo.lock = new std::mutex;
	}
	tcpInfo.bHandleAccptMsg = false;

	m_mutex.lock();	//����
	if (m_socketInfoVec[index].isConnect)
	{
		m_mutex.unlock(); //����
		Log(CERR, "��������ʧ��,fd=%d,ip=%s", fd, pTCPSocketInfo->ip);
		closesocket(fd);
		bufferevent_free(bev);
		SafeDelete(pRecvThreadParam);
		return;
	}
	m_socketInfoVec[index] = tcpInfo;
	m_heartBeatSocketSet.insert((unsigned int)index);
	m_uCurSocketSize++;
	m_mutex.unlock(); //����

	if (m_ServerIndex < 0 && m_ServerSock == fd)
	{
		m_ServerIndex = index;
	}

	Log(CINF, "TCP connect [ip=%s port=%d index=%d fd=%d bufferevent=%p]",
		tcpInfo.ip, tcpInfo.port, index, tcpInfo.acceptFd, tcpInfo.bev);
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
		Log(CERR, "�������Ѿ�����fd=%d [ip:%s %d][������%u/%u]", fd,
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
		Log(CERR, "Ͷ��������Ϣʧ��,fd=%d", fd);
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
		Log(INF, "�������� index=%d fd=%d", index, pThis->m_socketInfoVec[index].acceptFd);
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
bool CTCPSocketManage::VerifyConnection(char* data)
{
	if (!data)
	{
		return false;
	}
	int index = GetServerIndex();
	if (index < 0)
	{
		return false;
	}
	TCPSocketInfo* pTcpInfo = GetTCPSocketInfo(index);
	if (!pTcpInfo)
	{
		return false;
	}
	Netmsg is(data);
	std::string str;
	is >> str;
	if (str.empty())
	{
		return false;
	}
	char* pEncrypt = Util::Encrypt((char*)str.c_str(), str.size());
	if (!pEncrypt)
	{
		return false;
	}
	Netmsg msg;
	msg << pEncrypt;

	if (!SendMsg(GetServerIndex(), msg.str().c_str(), msg.str().size(), MsgCmd::MsgCmd_Testlink, 0, 0, pTcpInfo->bev))
	{
		return false;
	}

	return true;
}
// ������Ϣ�ɷ�
bool CTCPSocketManage::DispatchPacket(void* pBufferevent, int index, NetMessageHead* pHead, void* pData, int size, 
	SocketType socketType/* = SocketType::SOCKET_TYPE_TCP*/)
{
	if (!pBufferevent || !pHead)
	{
		return false;
	}
	TCPSocketInfo* pTcpInfo = GetTCPSocketInfo(index);
	if (!pTcpInfo)
	{
		return false;
	}
	if (pHead->uMainID == (unsigned int)MsgCmd::MsgCmd_HeartBeat) //������
	{
		return true;
	}
	if (pHead->uMainID == (unsigned int)MsgCmd::MsgCmd_Testlink) //�������Ӱ�
	{
		if (!VerifyConnection((char*)pData))
		{
			CloseSocket(index);
			return false;
		}
		m_IsConnect = true;
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
	// �ͷ����߳���ص���
	lock->lock();

	isConnect = false;
	bufferevent_free(bev);
	bev = nullptr;
	bHandleAccptMsg = false;

	// ���������߳�
	lock->unlock();
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
		Log(CERR, "index=%d ������Χ", index);
		return;
	}

	unsigned long uAccessIP = 0;

	// ����
	m_mutex.lock();
	// �ظ�����
	if (!tcpInfo->isConnect)
	{
		m_mutex.unlock();
		return;
	}
	// �����û�з����ڴ棬�ͷ���
	if (!tcpInfo->lock)
	{
		tcpInfo->lock = new std::mutex;
	}
	uAccessIP = inet_addr(tcpInfo->ip);
	m_uCurSocketSize--;
	m_heartBeatSocketSet.erase((unsigned int)index);
	// �ͷŲ����ڴ�
	RecvThreadParam* pRecvThreadParam = (RecvThreadParam*)0x01;
	bufferevent_getcb(tcpInfo->bev, nullptr, nullptr, nullptr, (void**)&pRecvThreadParam);
	if (pRecvThreadParam)
	{
		SafeDelete(pRecvThreadParam);
	}
	// �ͷ����߳���ص���
	tcpInfo->Reset(m_ServiceType);
	// �������߳�
	m_mutex.unlock();

	// ���û������BEV_OPT_CLOSE_ON_FREE ѡ���ر�socket
	closesocket(tcpInfo->acceptFd);

	// �������
	OnSocketCloseEvent(uAccessIP, index, (unsigned int)tcpInfo->acceptMsgTime, false);

	Log(CINF, "TCP close [ip=%s port=%d index=%d fd=%d isClientAutoClose:%d acceptTime=%lld]",
		tcpInfo->ip, tcpInfo->port, index, tcpInfo->acceptFd, isClientAutoClose, tcpInfo->acceptMsgTime);
}
//����رմ���
bool CTCPSocketManage::OnSocketCloseEvent(unsigned long uAccessIP, unsigned int uIndex,
	unsigned int uConnectTime, bool isCross, uint64_t userid/* = 0*/)
{
	SocketCloseLine SocketClose;
	SocketClose.uConnectTime = uConnectTime;
	SocketClose.uIndex = uIndex;
	SocketClose.uAccessIP = uAccessIP;
	return (m_pRecvDataLine->AddData(&SocketClose, sizeof(SocketClose),
		SysMsgCmd::HD_SOCKET_CLOSE) != 0);
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

	std::lock_guard<std::mutex> guard(m_mutex);

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
// ���������㷨
int CTCPSocketManage::GetSocketIndex()
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
	TCPSocketInfo* tcpInfo = GetTCPSocketInfo(index);
	if (!tcpInfo)
	{
		Log(CERR, "index=%d ������Χ", index);
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
				Log(CERR, "��������ʧ�ܣ�index=%d socketfd=%d bev=%p,", index, tcpInfo->acceptFd, tcpInfo->bev);
			}
		}
	}

	return true;
}
bool CTCPSocketManage::SendMsg(int index, const char* pData, size_t size, MsgCmd mainID, int assistID, int handleCode, 
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
			Log(CERR, "Ͷ����Ϣʧ��,mainID=%d,assistID=%d", mainID, assistID);
			return false;
		}
	}

	return true;
}

// �����߳���Ϣ������
void CTCPSocketManage::ThreadSendMsg()
{
	CDataLine* pDataLine = GetSendDataLine();
	if (!pDataLine)
	{
		Log(CERR, "send list is null");
		return;
	}
	ListItemData* pListItem = NULL;
	unsigned int uDataKind = 0;
	while (true)
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

	HandleSendData(pListItem);

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

// ������Ϣ���н������
bool CTCPSocketManage::RecvData(bufferevent* bev, int index)
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

	// ʣ�ദ������
	size_t handleRemainSize = realAllSize;

	// �����ͷ
	NetMessageHead* pNetHead = (NetMessageHead*)recvBuf.get();

	// �����ж�
	if (handleRemainSize >= sizeof(NetMessageHead) && pNetHead->uMessageSize > SOCKET_RECV_BUF_SIZE)
	{
		// ��Ϣ��ʽ����ȷ
		CloseSocket(index);
		Log(CERR, "��Ϣ��ʽ����ȷ,index=%d", index);
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
			Log(CERR, "��Ϣ��ʽ����ȷ");
			return false;
		}

		int realSize = messageSize - sizeof(NetMessageHead);
		if (realSize < 0)
		{
			// ���ݰ�������ͷ
			CloseSocket(index);
			Log(CERR, "���ݰ�������ͷ");
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

bool CTCPSocketManage::GetIsConnect()
{
	return m_IsConnect;
}