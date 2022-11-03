#pragma once

class CTCPSocketManage
{
public:
	// 构造函数
	CTCPSocketManage();
	virtual ~CTCPSocketManage();

protected:
	// 禁用拷贝构造函数
	CTCPSocketManage(const CTCPSocketManage& my);
	CTCPSocketManage& operator=(const CTCPSocketManage& my);

public:
	// 停止服务
	bool Stop();
	// 开始服务
	bool Start();
	// 初始化
	bool Init(int maxCount, ServiceType serverType = ServiceType::SERVICE_TYPE_BEGIN);

public:
	// 发送数据函数
	bool SendMsg(int index, const char* pData, size_t size, MsgCmd mainID, int assistID, int handleCode,
		void* pBufferevent, unsigned int uIdentification = 0, bool WSPackData = true);
	// 关闭连接函数
	bool CloseSocket(int index);
	// 设置tcp为未连接状态
	void RemoveTCPSocketStatus(int index, bool isClientAutoClose = false);
	// 连接DB服务器
	bool ConnectLogicServer(SOCKFD& sock);
	void ServerSocketInfo(PlatformSocketInfo* pTCPSocketInfo);
	// 连接服务器
	bool ConnectServer();
	// 等待连接
	bool WaitConnect(int threadIndex);
	// 获取套接字
	SOCKFD GetNewSocket();
	//网络关闭处理
	bool OnSocketCloseEvent(unsigned long uAccessIP, unsigned int uIndex,
		unsigned int uConnectTime, bool isCross, uint64_t userid = 0);

public:
	// 获取服务器类型
	ServiceType GetServerType();
	// 获取接收dataline
	CDataLine* GetRecvDataLine();
	// 获取发送dataline
	CDataLine* GetSendDataLine();
	// 获取运行线程
	std::vector<std::thread*>& GetSockeThreadVec();
	// 是否连接
	bool IsConnected(int index);
	// 获取socketSet
	void GetSocketSet(std::vector<unsigned int>& vec);
	// 获取TCPSocketInfo
	TCPSocketInfo* GetTCPSocketInfo(int index);
	// 获取socketVector
	const std::vector<TCPSocketInfo>& GetSocketVector();
	// 获取连接ip
	const char* GetSocketIP(int index);
	// 获取当前socket连接总数
	unsigned int GetCurSocketSize();

private:
	// 设置tcp属性
	// 设置tcp收发缓冲区
	static void SetTcpRcvSndBUF(SOCKFD fd, int rcvBufSize, int sndBufSize);
	// 设置应用层单次读取数据包的大小 bufferevent_set_max_single_read
	static void SetMaxSingleReadAndWrite(bufferevent* bev, int rcvBufSize, int sndBufSize);

private:
	// 线程函数
	// SOCKET 连接应答线程
	void ThreadAccept();
	// SOCKET 数据发送线程
	void ThreadSendMsg();
	// SOCKET 数据接收线程
	static void ThreadRSSocket(void* pThreadData);

private:
	// 静态回调方法
	// 新的数据到来，ThreadRSSocket线程函数
	static void ReadCB(struct bufferevent*, void*);
	// 连接关闭等等错误消息，ThreadRSSocket线程函数
	static void EventCB(struct bufferevent*, short, void*);
	// accept失败，ThreadAccept线程函数
	static void AcceptErrorCB(struct evconnlistener* listener, void*);
	// 新的连接到来，ThreadRSSocket线程函数
	static void ThreadLibeventProcess(evutil_socket_t readfd, short which, void* arg);
	// 新的连接到来，ThreadAccept线程函数
	static void ListenerCB(struct evconnlistener*, evutil_socket_t, struct sockaddr*, int socklen, void*);

private:
	// 分配socketIndex算法
	int GetSocketIndex();
	// 添加TCPSocketInfo
	void AddTCPSocketInfo(int threadIndex, PlatformSocketInfo* pTCPSocketInfo);
	// 派发数据包
	bool DispatchPacket(void* pBufferevent, int index, NetMessageHead* pHead, void* pData,
		int size, SocketType socketType = SocketType::SOCKET_TYPE_TCP);
	// 消息验证
	bool VerifyConnection(char* data);
private:
	// Socketpair
	static int DgramSocketpair(struct addrinfo* addr_info, SOCKFD sock[2]);
	static int StreamSocketpair(struct addrinfo* addr_info, SOCKFD sock[2]);
	static int Socketpair(int family, int type, int protocol, SOCKFD recv[2]);

private:
	// 发送TCP消息
	bool BuffereventWrite(int index, void* data, unsigned int size);
	// 最底层处理收到的数据函数
	bool RecvData(bufferevent* bev, int index);
	// 处理发送线程消息
	void HandleSendMsg(ListItemData* pListItem);
	// 处理发送线程消息
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