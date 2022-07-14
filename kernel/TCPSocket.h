#pragma once

class CTCPSocketManage
{
public:
	CTCPSocketManage();
	virtual ~CTCPSocketManage();

public:
	// ��ʼ��
	virtual bool Init(int maxCount, int port, const char* ip = NULL, SocketType socketType = SocketType::SOCKET_TYPE_TCP);
	// ȡ����ʼ��
	virtual bool UnInit();
	// ��ʼ����
	virtual bool Start(ServiceType serverType);
	// ֹͣ����
	virtual bool Stop();

public:
	// �������ݺ���
	virtual bool SendData(int index, const char* pData, size_t size, MsgCmd mainID, int assistID, int handleCode, void* pBufferevent, unsigned int uIdentification = 0);
	// �ر�����(ҵ���߼��̵߳���)
	bool CloseSocket(int index);

public:
	// ��ȡ����dataline
	CDataLine* GetRecvDataLine();
	// ��ȡ����dataline
	CDataLine* GetSendDataLine();
	// ��ȡ��ǰsocket��������
	unsigned int GetCurSocketSize();
	// �ж�socket�Ƿ�����
	bool IsConnected(int index);
	// ��ȡsocketSet
	void GetSocketSet(std::vector<unsigned int>& vec);
	const std::set<unsigned int>* GetSocketSet();
	// ��ȡsocketVector
	const std::vector<TCPSocketInfo>& GetSocketVector();
	// ��ȡ����ip
	const char* GetSocketIP(int index);
	// ��ȡTcpSocketInfo
	const TCPSocketInfo* GetTCPSocketInfo(int index);
	// ��ȡ�߳̾������
	std::vector<std::thread*>& GetSockeThreadVec();
	// ��ȡ�߳�����״̬
	bool GetRuninged();
	void SetRuninged(bool run);
	// ��ȡ����������
	ServiceType GetServerType();
	ConditionVariable& GetConditionVariable();
	event_base* GetEventBase();

private:
	// ����tcp����
	// ����tcp�շ�������
	static void SetTcpRcvSndBUF(SOCKET fd, int rcvBufSize, int sndBufSize);
	// ����Ӧ�ò㵥�ζ�ȡ���ݰ��Ĵ�С bufferevent_set_max_single_read
	static void SetMaxSingleReadAndWrite(bufferevent* bev, int rcvBufSize, int sndBufSize);

private:
	// ����socketIndex�㷨
	int GetSocketIndex();
	// ���TCPSocketInfo
	void AddTCPSocketInfo(int threadIndex, PlatformSocketInfo* pTCPSocketInfo);
	// ����tcpΪδ����״̬
	void RemoveTCPSocketStatus(int index, bool isClientAutoClose = false);
	// �ɷ����ݰ�
	bool DispatchPacket(void* pBufferevent, int index, NetMessageHead* pHead, void* pData, int size);
	// ��ײ㴦���յ������ݺ���
	bool RecvData(bufferevent* bev, int index);

private:
	// SOCKET ����Ӧ���߳�
	static void ThreadAcceptThread(void* pThreadData);
	// SOCKET ���ݽ����߳�
	static void ThreadRSSocketThread(void* pThreadData);
	// SOCKET ���ݷ����߳�
	static void ThreadSendMsgThread(void* pThreadData);

private:
	// �ص�����
	// �µ����ӵ�����ThreadAcceptThread�̺߳���
	static void ListenerCB(struct evconnlistener*, evutil_socket_t, struct sockaddr*, int socklen, void*);
	// �ͻ������ݶ�ȡ
	static void ReadCB(struct bufferevent*, void*);
	// ���ӹرյȵȴ�����Ϣ
	static void EventCB(struct bufferevent*, short, void*);
	// acceptʧ�ܣ�ThreadAcceptThread�̺߳���
	static void AcceptErrorCB(struct evconnlistener* listener, void*);
	// �µ����ӵ�����ThreadRSSocketThread�̺߳���
	static void ThreadLibeventProcess(evutil_socket_t readfd, short which, void* arg);

private:
	//�Զ���socketPair
	static int StreamSocketpair(struct addrinfo* addr_info, SOCKET sock[2]);
	static int DgramSocketpair(struct addrinfo* addr_info, SOCKET sock[2]);
	static int Socketpair(int family, int type, int protocol, SOCKET recv[2]);

private:
	event_base*                 m_listenerBase;
	std::vector<WorkThreadInfo> m_workBaseVec;
	ConditionVariable           m_ConditionVariable;
	CDataLine*                  m_pSendDataLine;	//���Ͷ���
	CDataLine*                  m_pRecvDataLine;	//���ܶ���
	std::vector<TCPSocketInfo>  m_socketInfoVec;
	std::set<unsigned int>      m_heartBeatSocketSet;
	bool                        m_running;
	char                        m_bindIP[48];
	unsigned short              m_port;
	SocketType                  m_socketType;
	unsigned int                m_uMaxSocketSize; // libevent ���߳�Ĭ�ϵ�32000
	unsigned int                m_uCurSocketSize;
	unsigned int                m_uCurSocketIndex;
	ServiceType                 m_iServiceType;
	std::vector<std::thread*>   m_socketThread;
};