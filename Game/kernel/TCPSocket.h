#pragma once

class CTCPSocketManage
{
public:
	CTCPSocketManage();
	virtual ~CTCPSocketManage();

protected:
	// ���ÿ������캯��
	CTCPSocketManage(const CTCPSocketManage& my);
	CTCPSocketManage& operator=(const CTCPSocketManage& my);

public:
	// ֹͣ����
	bool Stop();
	// ��ʼ����
	bool Start();
	// ��ʼ��
	bool Init(int maxCount, int port, const char* ip = nullptr, 
		ServiceType serverType = ServiceType::SERVICE_TYPE_BEGIN);

public:
	// �ر����Ӻ���
	bool CloseSocket(int index);
	// �������ݺ���
	bool SendMsg(int index, const char* pData, size_t size, MsgCmd mainID, int assistID, int handleCode, 
		void* pBufferevent, unsigned int uIdentification = 0, bool WSPackData = true);
	// ����tcpΪδ����״̬
	void RemoveTCPSocketStatus(int index, bool isClientAutoClose = false);

public:
	// ��ȡevent_base
	event_base* GetEventBase();
	// ��ȡ����������
	ServiceType GetServerType();
	// ��ȡ����dataline
	CDataLine* GetRecvDataLine();
	// ��ȡ����dataline
	CDataLine* GetSendDataLine();
	// ��ȡ��������
	ConditionVariable& GetConditionVariable();
	// ��ȡ�����߳�
	std::vector<std::thread*>& GetSockeThreadVec();
	// ��ȡ����״̬
	bool& GetRuninged();
	// �Ƿ�����
	bool IsConnected(int index);
	// ��ȡsocketSet
	void GetSocketSet(std::vector<unsigned int>& vec);
	// ��ȡTCPSocketInfo
	TCPSocketInfo* GetTCPSocketInfo(int index);
	// ��ȡsocketVector
	const std::vector<TCPSocketInfo>& GetSocketVector();
	// ��ȡ����ip
	const char* GetSocketIP(int index);
	// ��ȡ��ǰsocket��������
	unsigned int GetCurSocketSize();

private:
	// ����tcp����
	// ����tcp�շ�������
	static void SetTcpRcvSndBUF(SOCKFD fd, int rcvBufSize, int sndBufSize);
	// ����Ӧ�ò㵥�ζ�ȡ���ݰ��Ĵ�С bufferevent_set_max_single_read
	static void SetMaxSingleReadAndWrite(bufferevent* bev, int rcvBufSize, int sndBufSize);

private:
	// ����socketIndex�㷨
	int GetSocketIndex();
	// ���TCPSocketInfo
	void AddTCPSocketInfo(int threadIndex, PlatformSocketInfo* pTCPSocketInfo);
	// �ɷ����ݰ�
	bool DispatchPacket(void* pBufferevent, int index, NetMessageHead* pHead, void* pData, 
		int size, SocketType socketType = SocketType::SOCKET_TYPE_TCP);
	//����رմ���
	bool OnSocketCloseEvent(unsigned long uAccessIP, unsigned int uIndex, unsigned int uConnectTime);

private:
	// �̺߳���
	// SOCKET ����Ӧ���߳�
	void ThreadAccept();
	// SOCKET ���ݷ����߳�
	void ThreadSendMsg();
	// SOCKET ���ݽ����߳�
	static void ThreadRSSocket(void* pThreadData);

private:
	// ��̬�ص�����
	// �µ����ݵ�����ThreadRSSocket�̺߳���
	static void ReadCB(struct bufferevent*, void*);
	// ���ӹرյȵȴ�����Ϣ��ThreadRSSocket�̺߳���
	static void EventCB(struct bufferevent*, short, void*);
	// acceptʧ�ܣ�ThreadAccept�̺߳���
	static void AcceptErrorCB(struct evconnlistener* listener, void*);
	// �µ����ӵ�����ThreadRSSocket�̺߳���
	static void ThreadLibeventProcess(evutil_socket_t readfd, short which, void* arg);
	// �µ����ӵ�����ThreadAccept�̺߳���
	static void ListenerCB(struct evconnlistener*, evutil_socket_t, struct sockaddr*, int socklen, void*);

private:
	// Socketpair
	static int DgramSocketpair(struct addrinfo* addr_info, SOCKFD sock[2]);
	static int StreamSocketpair(struct addrinfo* addr_info, SOCKFD sock[2]);
	static int Socketpair(int family, int type, int protocol, SOCKFD recv[2]);

private:
	// �ԳƼ��ܲ�������
	bool VerifyConnection(int index, char* data);

private:
	// д��bufferevent_write
	bool BuffereventWrite(int index, std::string& data);
	// д��bufferevent_write
	bool BuffereventWrite(int index, void* data, unsigned int size);

	// ����TCP��Ϣ
	bool SendLogicMsg(int index, const char* pData, size_t size, MsgCmd mainID, int assistID, int handleCode, 
		void* pBufferevent, unsigned int uIdentification = 0);
#ifdef __WebSocket__
	bool WSSendWSLogicMsg(int index, const char* pData, size_t size, MsgCmd mainID, int assistID, int handleCode,
		void* pBufferevent, unsigned int uIdentification = 0, bool PackData = true);
#endif
#ifdef __WebSocketOpenssl__
	bool WSSSendWSLogicMsg(int index, const char* pData, size_t size, MsgCmd mainID, int assistID, int handleCode,
		void* pBufferevent, unsigned int uIdentification = 0, bool PackData = true);
#endif

	// ��ײ㴦���յ������ݺ���
	bool RecvData(bufferevent* bev, int index);
	// �����յ���Ϣ����ճ��
	bool RecvLogicData(bufferevent* bev, int index);
#ifdef __WebSocket__
	bool WSRecvWSLogicData(bufferevent* bev, int index);
#endif
#ifdef __WebSocketOpenssl__
	bool WSSRecvWSSLogicData(bufferevent* bev, int index);
#endif

	// �������߳���Ϣ
	void HandleSendMsg(ListItemData* pListItem);
	// �������߳���Ϣ
	void HandleSendData(ListItemData* pListItem);
#ifdef __WebSocket__
	void WSHandleSendWSData(ListItemData * pListItem);
#endif
#ifdef __WebSocketOpenssl__
	void WSSHandleSendWSData(ListItemData* pListItem);
#endif

	// openssl ����
#ifdef __WebSocketOpenssl__
private:
	bool WSSOpensslInit();
	//SSL* WSSCreateSSL(evutil_socket_t fd);
	//bool WSSOpensslHandShark(int index);
#endif

	// websocket�ĵ�һ������
#ifdef __WebSocket__
	bool WSHandShark(bufferevent* bev, int index);
#endif

	// websocket�������ݰ�����
#ifdef __WebSocket__
private:
	static int WSFetchFin(char* msg, int& pos, WebSocketMsg& wbmsg);
	static int WSFetchOpcode(char* msg, int& pos, WebSocketMsg& wbmsg);
	static int WSFetchMask(char* msg, int& pos, WebSocketMsg& wbmsg);
	static int WSFetchMaskingKey(char* msg, int& pos, WebSocketMsg& wbmsg);
	static int WSFetchPayloadLength(char* msg, int& pos, WebSocketMsg& wbmsg);
	static int WSFetchPayload(char* msg, int& pos, WebSocketMsg& wbmsg);
	static void WSFetchPrint(const WebSocketMsg& wbmsg);
#endif

private:
	bool				 m_running;
	char				 m_bindIP[48];
	unsigned short		 m_port;
	unsigned int		 m_uMaxSocketSize;
	unsigned int		 m_uCurSocketSize;
	unsigned int		 m_uCurSocketIndex;
	CDataLine*			 m_pRecvDataLine;
	CDataLine*			 m_pSendDataLine;
	event_config*		 m_eventBaseCfg;
	event_base*			 m_listenerBase;
	ServiceType			 m_iServiceType;
	ConditionVariable	 m_ConditionVariable;

	SetUint				 m_heartBeatSocketSet;
	VectorThread		 m_socketThread;
	VectorTCPSocketInfo	 m_socketInfoVec;
	VectorWorkThreadInfo m_workBaseVec;

	SSL_CTX*		     m_ctx;
};