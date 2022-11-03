#pragma once

class CTCPSocketManage
{
public:
	// ���캯��
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
	bool Init(int maxCount, ServiceType serverType = ServiceType::SERVICE_TYPE_BEGIN);

public:
	// �������ݺ���
	bool SendMsg(int index, const char* pData, size_t size, MsgCmd mainID, int assistID, int handleCode,
		void* pBufferevent, unsigned int uIdentification = 0, bool WSPackData = true);
	// �ر����Ӻ���
	bool CloseSocket(int index);
	// ����tcpΪδ����״̬
	void RemoveTCPSocketStatus(int index, bool isClientAutoClose = false);
	// ����DB������
	bool ConnectLogicServer(SOCKFD& sock);
	void ServerSocketInfo(PlatformSocketInfo* pTCPSocketInfo);
	// ���ӷ�����
	bool ConnectServer();
	// �ȴ�����
	bool WaitConnect(int threadIndex);
	// ��ȡ�׽���
	SOCKFD GetNewSocket();
	//����رմ���
	bool OnSocketCloseEvent(unsigned long uAccessIP, unsigned int uIndex,
		unsigned int uConnectTime, bool isCross, uint64_t userid = 0);

public:
	// ��ȡ����������
	ServiceType GetServerType();
	// ��ȡ����dataline
	CDataLine* GetRecvDataLine();
	// ��ȡ����dataline
	CDataLine* GetSendDataLine();
	// ��ȡ�����߳�
	std::vector<std::thread*>& GetSockeThreadVec();
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
	// ����socketIndex�㷨
	int GetSocketIndex();
	// ���TCPSocketInfo
	void AddTCPSocketInfo(int threadIndex, PlatformSocketInfo* pTCPSocketInfo);
	// �ɷ����ݰ�
	bool DispatchPacket(void* pBufferevent, int index, NetMessageHead* pHead, void* pData,
		int size, SocketType socketType = SocketType::SOCKET_TYPE_TCP);
	// ��Ϣ��֤
	bool VerifyConnection(char* data);
private:
	// Socketpair
	static int DgramSocketpair(struct addrinfo* addr_info, SOCKFD sock[2]);
	static int StreamSocketpair(struct addrinfo* addr_info, SOCKFD sock[2]);
	static int Socketpair(int family, int type, int protocol, SOCKFD recv[2]);

private:
	// ����TCP��Ϣ
	bool BuffereventWrite(int index, void* data, unsigned int size);
	// ��ײ㴦���յ������ݺ���
	bool RecvData(bufferevent* bev, int index);
	// �������߳���Ϣ
	void HandleSendMsg(ListItemData* pListItem);
	// �������߳���Ϣ
	void HandleSendData(ListItemData* pListItem);

public:
	bool GetIsConnect();

public:
	static	bool		 m_running;
private:
	unsigned int		 m_uMaxSocketSize;
	unsigned int		 m_uCurSocketSize;
	unsigned int		 m_uCurSocketIndex;
	CDataLine*			 m_pRecvDataLine;
	CDataLine*			 m_pSendDataLine;
	ServiceType			 m_ServiceType;
	std::mutex			 m_mutex;

	SetUint				 m_heartBeatSocketSet;
	VectorThread		 m_socketThread;
	VectorTCPSocketInfo	 m_socketInfoVec;
	VectorWorkThreadInfo m_workBaseVec;
	bool				 m_IsConnect;
	RecvThreadParam*	 m_RecvThreadParam;

	BuildReference(ClientInfo, ClientInfo)
	BuildValue(int, ServerIndex)
	BuildValue(SOCKFD, ServerSock)
};