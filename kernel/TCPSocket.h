#pragma once

class CTCPSocketManage
{
public:
	CTCPSocketManage();
	virtual ~CTCPSocketManage();

protected:
	CTCPSocketManage(const CTCPSocketManage& my);
	CTCPSocketManage& operator=(const CTCPSocketManage& my);

public:
	bool Stop();
	bool Start(ServiceType serverType);
	bool Init(int maxCount, int port, const char* ip = nullptr);

public:
	bool CloseSocket(int index);
	bool SendData(int index, const char* pData, size_t size, MsgCmd mainID, int assistID, int handleCode, void* pBufferevent, unsigned int uIdentification = 0);
	void RemoveTCPSocketStatus(int index, bool isClientAutoClose = false);

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

	const TCPSocketInfo* GetTCPSocketInfo(int index);
	const std::vector<TCPSocketInfo>& GetSocketVector();

	const char* GetSocketIP(int index);
	unsigned int GetCurSocketSize();

private:
	static void SetTcpRcvSndBUF(SOCKFD fd, int rcvBufSize, int sndBufSize);
	static void SetMaxSingleReadAndWrite(bufferevent* bev, int rcvBufSize, int sndBufSize);

private:
	int GetSocketIndex();
	void AddTCPSocketInfo(int threadIndex, PlatformSocketInfo* pTCPSocketInfo);
	bool DispatchPacket(void* pBufferevent, int index, NetMessageHead* pHead, void* pData, int size, SocketType socketType = SocketType::SOCKET_TYPE_TCP);
	bool OnSocketCloseEvent(unsigned long uAccessIP, unsigned int uIndex, unsigned int uConnectTime);

private:
	// 线程入口
	void ThreadAcceptThread();
	void ThreadSendMsgThread();
	void HandleSendData(ListItemData* pListItem);
	static void ThreadRSSocketThread(void* pThreadData);

private:
	// 静态回调方法
	static void ReadCB(struct bufferevent*, void*);
	static void EventCB(struct bufferevent*, short, void*);
	static void AcceptErrorCB(struct evconnlistener* listener, void*);
	static void ThreadLibeventProcess(evutil_socket_t readfd, short which, void* arg);
	static void ListenerCB(struct evconnlistener*, evutil_socket_t, struct sockaddr*, int socklen, void*);

private:
	// Socketpair
	static int DgramSocketpair(struct addrinfo* addr_info, SOCKFD sock[2]);
	static int StreamSocketpair(struct addrinfo* addr_info, SOCKFD sock[2]);
	static int Socketpair(int family, int type, int protocol, SOCKFD recv[2]);

private:
	// 对称加密测试连接
	bool VerifyConnection(int index, char* data);

	// 收到消息进行粘包处理
	bool RecvData(bufferevent* bev, int index);
	bool ServiceTypeLogic(bufferevent* bev, int index);
#ifdef __WebSocket__
private:
	bool ServiceTypeLogicWS(bufferevent* bev, int index);
	bool HandShark(bufferevent* bev, int index);

	static int FetchFin(char* msg, int& pos, WebSocketMsg& wbmsg);
	static int FetchOpcode(char* msg, int& pos, WebSocketMsg& wbmsg);
	static int FetchMask(char* msg, int& pos, WebSocketMsg& wbmsg);
	static int FetchMaskingKey(char* msg, int& pos, WebSocketMsg& wbmsg);
	static int FetchPayloadLength(char* msg, int& pos, WebSocketMsg& wbmsg);
	static int FetchPayload(char* msg, int& pos, WebSocketMsg& wbmsg);
	static void FetchPrint(const WebSocketMsg& wbmsg);
#endif // __WebSocket__

private:
	unsigned short              m_port;
	bool                        m_running;
	char                        m_bindIP[48];
	unsigned int                m_uMaxSocketSize;
	unsigned int                m_uCurSocketSize;
	unsigned int                m_uCurSocketIndex;
	CDataLine*					m_pRecvDataLine;
	CDataLine*					m_pSendDataLine;
	event_config*				m_eventBaseCfg;
	event_base*					m_listenerBase;
	ServiceType                 m_iServiceType;
	ConditionVariable           m_ConditionVariable;
	std::set<unsigned int>      m_heartBeatSocketSet;
	std::vector<std::thread*>   m_socketThread;
	std::vector<TCPSocketInfo>  m_socketInfoVec;
	std::vector<WorkThreadInfo> m_workBaseVec;
};