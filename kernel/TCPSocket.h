#pragma once

class CTCPSocketManage
{
public:
	CTCPSocketManage();
	virtual ~CTCPSocketManage();

public:
	bool Stop();
	bool UnInit();
	bool Start(ServiceType serverType);
	bool Init(int maxCount, int port, const char* ip = nullptr, SocketType socketType = SocketType::SOCKET_TYPE_TCP);

public:
	bool CloseSocket(int index);
	bool SendData(int index, const char* pData, size_t size, MsgCmd mainID, int assistID, int handleCode, void* pBufferevent, unsigned int uIdentification = 0);

public:
	event_base* GetEventBase();
	ServiceType GetServerType();
	CDataLine* GetRecvDataLine();
	CDataLine* GetSendDataLine();
	ConditionVariable& GetConditionVariable();
	std::vector<std::thread*>& GetSockeThreadVec();

	bool& GetRuninged();
	bool IsConnected(int index);
	void GetSocketSet(std::vector<unsigned int>& vec);

	const std::set<unsigned int>* GetSocketSet();
	const TCPSocketInfo* GetTCPSocketInfo(int index);
	const std::vector<TCPSocketInfo>& GetSocketVector();

	const char* GetSocketIP(int index);
	unsigned int GetCurSocketSize();

private:
	static void SetTcpRcvSndBUF(SOCKET fd, int rcvBufSize, int sndBufSize);
	static void SetMaxSingleReadAndWrite(bufferevent* bev, int rcvBufSize, int sndBufSize);

private:
	int GetSocketIndex();
	bool RecvData(bufferevent* bev, int index);
	void RemoveTCPSocketStatus(int index, bool isClientAutoClose = false);
	void AddTCPSocketInfo(int threadIndex, PlatformSocketInfo* pTCPSocketInfo);
	bool DispatchPacket(void* pBufferevent, int index, NetMessageHead* pHead, void* pData, int size);
	bool OnSocketCloseEvent(ULONG uAccessIP, UINT uIndex, UINT uConnectTime, BYTE socketType);

private:
	static void ThreadAcceptThread(void* pThreadData);
	static void ThreadRSSocketThread(void* pThreadData);
	static void ThreadSendMsgThread(void* pThreadData);

private:
	static void ReadCB(struct bufferevent*, void*);
	static void EventCB(struct bufferevent*, short, void*);
	static void AcceptErrorCB(struct evconnlistener* listener, void*);
	static void ThreadLibeventProcess(evutil_socket_t readfd, short which, void* arg);
	static void ListenerCB(struct evconnlistener*, evutil_socket_t, struct sockaddr*, int socklen, void*);

private:
	static int DgramSocketpair(struct addrinfo* addr_info, SOCKET sock[2]);
	static int StreamSocketpair(struct addrinfo* addr_info, SOCKET sock[2]);
	static int Socketpair(int family, int type, int protocol, SOCKET recv[2]);

private:
	unsigned short              m_port;
	bool                        m_running;
	char                        m_bindIP[48];
	unsigned int                m_uMaxSocketSize; // libevent 单线程默认的32000
	unsigned int                m_uCurSocketSize;
	unsigned int                m_uCurSocketIndex;
	CDataLine*					m_pRecvDataLine;	//接受对列
	CDataLine*					m_pSendDataLine;	//发送队列
	event_base*					m_listenerBase;
	SocketType                  m_socketType;
	ServiceType                 m_iServiceType;
	ConditionVariable           m_ConditionVariable;
	std::set<unsigned int>      m_heartBeatSocketSet;
	std::vector<std::thread*>   m_socketThread;
	std::vector<TCPSocketInfo>  m_socketInfoVec;
	std::vector<WorkThreadInfo> m_workBaseVec;
};