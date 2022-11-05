#pragma once

class TCPSocket
{
public:
	// 构造函数
	TCPSocket();
	virtual ~TCPSocket();

protected:
	// 禁用拷贝构造函数
	TCPSocket(const TCPSocket& my);
	TCPSocket& operator=(const TCPSocket& my);

public:
	// 停止服务
	bool Stop();
	// 开始服务
	bool Start();
	// 初始化
	bool Init(int maxCount, int port, const char* ip = nullptr,
		ServiceType serverType = ServiceType::SERVICE_TYPE_BEGIN);

public:
	// 关闭连接函数
	bool CloseSocket(int index);
	//网络关闭处理
	bool OnSocketCloseEvent(unsigned long uAccessIP, unsigned int uIndex,
		unsigned int uConnectTime, bool isCross, uint64_t userid = 0);
private:
	//网络关闭处理
	bool OnLogicSocketCloseEvent(unsigned long uAccessIP, unsigned int uIndex,
		unsigned int uConnectTime, bool isCross);
	//网络关闭处理
	bool OnCrossSocketCloseEvent(unsigned long uAccessIP, unsigned int uIndex,
		unsigned int uConnectTime, bool isCross, uint64_t& userid);
	// 设置tcp为未连接状态
	void RemoveTCPSocketStatus(int index, bool isClientAutoClose = false);

private:
	// 线程函数
	// SOCKET 连接应答线程
	void ThreadAccept();
	// SOCKET 数据发送线程
	void ThreadSendMsg();
	// SOCKET 数据接收线程
	static void ThreadRSSocket(void* pThreadData);

private:
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
	// 派发数据包
	bool DispatchPacket(void* pBufferevent, int index, NetMessageHead* pHead, void* pData,
		int size, SocketType socketType = SocketType::SOCKET_TYPE_TCP);
	// 派发数据包
	bool DispatchCrossPacket(void* pBufferevent, int index, NetMessageHead* pHead, void* pData,
		int size, SocketType socketType = SocketType::SOCKET_TYPE_TCP);
	// 派发数据包
	bool DispatchLogicPacket(void* pBufferevent, int index, NetMessageHead* pHead, void* pData,
		int size, SocketType socketType = SocketType::SOCKET_TYPE_TCP);

public:
	// 连接服务器
	bool ConnectServer();
	// 休眠
	void Sleepseconds(int seconds);
private:
	// 对称加密测试连接
	bool VerifyConnection(int index, char* data, int len);
	// 等待连接
	bool WaitConnect(int threadIndex);
	// 初始化跨服信息
	bool InitCrossServer();
	// 初始化DB信息
	bool InitDBServer();
	// 连接DB服务器
	bool ConnectDBServer(SOCKFD& sock);
	// 连接跨服服务器
	bool ConnectCrossServer(SOCKFD& sock);
	// 转到内部消息
	void ServerSocketInfo(PlatformSocketInfo* pTCPSocketInfo);
	// 获取套接字
	SOCKFD GetNewSocket();

public:
	// 发送数据函数.
	bool SendMsg(int index, const char* pData, size_t size, MsgCmd mainID, int assistID, int handleCode,
		void* pBufferevent, unsigned int uIdentification = 0, uint64_t userid = 0, bool WSPackData = true);
private:
	// 向跨服发送消息或者客户端
	bool SendCrossMsg(int index, const char* pData, size_t size, MsgCmd mainID, int assistID, int handleCode,
		void* pBufferevent, unsigned int uIdentification = 0, uint64_t userid = 0, bool WSPackData = true);
	// 发送TCP消息
	bool SendLogicMsg(int index, const char* pData, size_t size, MsgCmd mainID, int assistID, int handleCode,
		void* pBufferevent, unsigned int uIdentification = 0, uint64_t userid = 0);
	// 发送TCP消息
	bool SendLogicWsMsg(int index, const char* pData, size_t size, MsgCmd mainID, int assistID, int handleCode,
		void* pBufferevent, unsigned int uIdentification = 0, bool PackData = true);
	// 发送TCP消息
	bool SendLogicWssMsg(int index, const char* pData, size_t size, MsgCmd mainID, int assistID, int handleCode,
		void* pBufferevent, unsigned int uIdentification = 0, bool PackData = true);
	// 消息发送
	bool BuffereventWrite(int index, void* data, unsigned int size);

private:
	// 处理发送线程消息
	void HandleSendMsg(ListItemData* pListItem);
	// 处理发送线程消息
	void HandleSendData(ListItemData* pListItem);
	// 处理发送线程消息
	void HandleSendWsData(ListItemData* pListItem);
	// 处理发送线程消息
	void HandleSendWssData(ListItemData* pListItem);

private:
	// 最底层处理收到的数据函数
	bool RecvData(bufferevent* bev, int index);
	// 处理收到消息进行粘包
	bool RecvLogicData(bufferevent* bev, int index);
	// 处理收到消息进行粘包
	bool RecvLogicWsData(bufferevent* bev, int index);
	// 处理收到消息进行粘包
	bool RecvLogicWssData(bufferevent* bev, int index);

private:
	// 跨服消息转发
	bool MsgForward(int index, NetMessageHead* pHead, char* pData, int len);
	// 客户端消息转发到跨服服务器
	bool MsgForwardToCross(int index, NetMessageHead* pHead, char* pData, int len);
	// 跨服消息转发到客户端
	bool MsgForwardToClient(int index, NetMessageHead* pHead, char* pData, int len);

public:
	// 是否是内部服务器索引
	bool IsServerIndex(int index);
private:
	// 添加TCPSocketInfo
	void AddTCPSocketInfo(int threadIndex, PlatformSocketInfo* pTCPSocketInfo);
	// 获取bufferevent
	struct bufferevent* GetBufferEvent(struct event_base* base, SOCKFD& fd, SSL* ssl);
	// 设置内部服务器index
	void SetServerIndex(SOCKFD& fd, int index);
	// 设置客户端心跳
	void SetHeartbeat(struct bufferevent* bev, int index);
	// 保存TCPSocketInfo
	bool SaveTCPSocketInfo(PlatformSocketInfo* pTCPSocketInfo, struct bufferevent* bev, int index, TCPSocketInfo& tcpInfo);
	// AddTCPSocketInfo后后续处理
	void AddTCPSocketInfoAfter(int index, TCPSocketInfo& tcpInfo, SSL* ssl);

private:
	// 初始化openssl
	bool OpensslInit();
	// websocket的第一次握手
	bool HandShark(bufferevent* bev, int index);

public:
	// 分配socketIndex算法
	int GetSocketIndex();
	// 获取DBServer索引
	int GetDBServerIndex();
	// 获取跨服服务器索引
	int GetCrossServerIndex();
	// 获取运行状态
	bool& GetRuninged();
	// 是否连接
	bool IsConnected(int index);
	// 获取当前socket连接总数
	unsigned int GetCurSocketSize();
	// 获取socketSet
	void GetSocketSet(std::vector<unsigned int>& vec);
	// 获取连接ip
	const char* GetSocketIP(int index);
	// 获取socketVector
	const std::vector<TCPSocketInfo>& GetSocketVector();
	// 获取event_base
	event_base* GetEventBase();
	// 获取服务器类型
	ServiceType GetServerType();
	// 获取接收dataline
	CDataLine* GetRecvDataLine();
	// 获取发送dataline
	CDataLine* GetSendDataLine();
	// 获取运行线程
	std::vector<std::thread*>& GetSockeThreadVec();
	// 获取TCPSocketInfo
	TCPSocketInfo* GetTCPSocketInfo(int index);

private:
	// 设置tcp属性
	// 设置tcp收发缓冲区
	static void SetTcpRcvSndBUF(SOCKFD fd, int rcvBufSize, int sndBufSize);
	// 设置应用层单次读取数据包的大小 bufferevent_set_max_single_read
	static void SetMaxSingleReadAndWrite(bufferevent* bev, int rcvBufSize, int sndBufSize);

private:
	// websocket解析数据包函数
	static int FetchFin(char* msg, int& pos, WebSocketMsg& wbmsg);
	static int FetchOpcode(char* msg, int& pos, WebSocketMsg& wbmsg);
	static int FetchMask(char* msg, int& pos, WebSocketMsg& wbmsg);
	static int FetchMaskingKey(char* msg, int& pos, WebSocketMsg& wbmsg);
	static int FetchPayloadLength(char* msg, int& pos, WebSocketMsg& wbmsg);
	static int FetchPayload(char* msg, int& pos, WebSocketMsg& wbmsg);
	static void FetchPrint(const WebSocketMsg& wbmsg);

private:
	// Socketpair
	static int DgramSocketpair(struct addrinfo* addr_info, SOCKFD sock[2]);
	static int StreamSocketpair(struct addrinfo* addr_info, SOCKFD sock[2]);
	static int Socketpair(int family, int type, int protocol, SOCKFD recv[2]);

private:
	bool				 m_running;
	char				 m_bindIP[48];
	unsigned short		 m_port;
	unsigned int		 m_uMaxSocketSize;
	unsigned int		 m_uCurSocketSize;
	unsigned int		 m_uCurSocketIndex;
	CDataLine*			 m_pRecvDataLine;
	CDataLine*           m_pSendDataLine;
	event_config*        m_eventBaseCfg;
	event_base*			 m_listenerBase;
	ServiceType			 m_ServiceType;
	std::mutex			 m_mutex;
	SetUint				 m_heartBeatSocketSet;
	VectorThread		 m_socketThread;
	VectorTCPSocketInfo	 m_socketInfoVec;
	VectorWorkThreadInfo m_workBaseVec;
	SSL_CTX* m_ctx;
	SOCKFD				 m_DBServerSock;
	int					 m_DBServerIndex;
	SOCKFD				 m_CrossServerSock;
	int					 m_CrossServerIndex;
	static int			 m_lastThreadIndex;
};