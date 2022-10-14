#pragma once

class CTCPSocketManage
{
public:
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
	bool Start(ServiceType serverType);
	// 初始化
	bool Init(int maxCount, int port, const char* ip = nullptr);

public:
	// 关闭连接函数
	bool CloseSocket(int index);
	// 发送数据函数
	bool SendMsg(int index, const char* pData, size_t size, MsgCmd mainID, int assistID, int handleCode, 
		void* pBufferevent, unsigned int uIdentification = 0, bool WSPackData = true);
	// 设置tcp为未连接状态
	void RemoveTCPSocketStatus(int index, bool isClientAutoClose = false);

public:
	// 获取event_base
	event_base* GetEventBase();
	// 获取服务器类型
	ServiceType GetServerType();
	// 获取接收dataline
	CDataLine* GetRecvDataLine();
	// 获取发送dataline
	CDataLine* GetSendDataLine();
	// 获取条件变量
	ConditionVariable& GetConditionVariable();
	// 获取运行线程
	std::vector<std::thread*>& GetSockeThreadVec();
	// 获取运行状态
	bool& GetRuninged();
	// 是否连接
	bool IsConnected(int index);
	// 获取socketSet
	void GetSocketSet(std::vector<unsigned int>& vec);
	// 获取TCPSocketInfo
	const TCPSocketInfo* GetTCPSocketInfo(int index);
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
	// 分配socketIndex算法
	int GetSocketIndex();
	// 添加TCPSocketInfo
	void AddTCPSocketInfo(int threadIndex, PlatformSocketInfo* pTCPSocketInfo);
	// 派发数据包
	bool DispatchPacket(void* pBufferevent, int index, NetMessageHead* pHead, void* pData, 
		int size, SocketType socketType = SocketType::SOCKET_TYPE_TCP);
	//网络关闭处理
	bool OnSocketCloseEvent(unsigned long uAccessIP, unsigned int uIndex, unsigned int uConnectTime);

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
	// Socketpair
	static int DgramSocketpair(struct addrinfo* addr_info, SOCKFD sock[2]);
	static int StreamSocketpair(struct addrinfo* addr_info, SOCKFD sock[2]);
	static int Socketpair(int family, int type, int protocol, SOCKFD recv[2]);

private:
	// 对称加密测试连接
	bool VerifyConnection(int index, char* data);

private:
	// 写入bufferevent_write
	bool BuffereventWrite(int index, std::string& data);
	// 写入bufferevent_write
	bool BuffereventWrite(int index, void* data, unsigned int size);

	// 发送TCP消息
	bool SendLogicMsg(int index, const char* pData, size_t size, MsgCmd mainID, int assistID, int handleCode, 
		void* pBufferevent, unsigned int uIdentification = 0);
#ifdef __WebSocket__
	// 发送WebSocket消息
	bool WSSendWSLogicMsg(int index, const char* pData, size_t size, MsgCmd mainID, int assistID, int handleCode,
		void* pBufferevent, unsigned int uIdentification = 0, bool PackData = true);
#endif // __WebSocket__

	// 最底层处理收到的数据函数
	bool RecvData(bufferevent* bev, int index);
	// 处理收到消息进行粘包
	bool RecvLogicData(bufferevent* bev, int index);
#ifdef __WebSocket__
	// 收到消息进行粘包处理
	bool WSRecvWSLogicData(bufferevent* bev, int index);
#endif // __WebSocket__

	// 处理发送线程消息
	void HandleSendMsg(ListItemData* pListItem);
	// 处理发送线程消息
	void HandleSendData(ListItemData* pListItem);
#ifdef __WebSocket__
	// 处理发送线程消息
	void WSHandleSendWSData(ListItemData * pListItem);
#endif // __WebSocket__

#ifdef __WebSocket__
private:
	// 加载证书
	bool WSOpensslCert(std::string path);
	// opensslInit
	bool WSOpensslInit();
	// 进行openssl握手
	bool WSOpensslHandShark(bufferevent* bev, int index);
#endif // __WebSocket__

#ifdef __WebSocket__
	// websocket的第一次握手
	bool WSHandShark(bufferevent* bev, int index);
#endif // __WebSocket__

#ifdef __WebSocket__
private:
	// websocket解析数据包函数
	static int WSFetchFin(char* msg, int& pos, WebSocketMsg& wbmsg);
	static int WSFetchOpcode(char* msg, int& pos, WebSocketMsg& wbmsg);
	static int WSFetchMask(char* msg, int& pos, WebSocketMsg& wbmsg);
	static int WSFetchMaskingKey(char* msg, int& pos, WebSocketMsg& wbmsg);
	static int WSFetchPayloadLength(char* msg, int& pos, WebSocketMsg& wbmsg);
	static int WSFetchPayload(char* msg, int& pos, WebSocketMsg& wbmsg);
	static void WSFetchPrint(const WebSocketMsg& wbmsg);
#endif // __WebSocket__

private:
	unsigned short	m_port;
	bool			m_running;
	char			m_bindIP[48];
	unsigned int	m_uMaxSocketSize;
	unsigned int	m_uCurSocketSize;
	unsigned int	m_uCurSocketIndex;
	CDataLine*		m_pRecvDataLine;
	CDataLine*		m_pSendDataLine;
	event_config*	m_eventBaseCfg;
	event_base*		m_listenerBase;
	ServiceType		m_iServiceType;

	ConditionVariable           m_ConditionVariable;
	std::set<unsigned int>      m_heartBeatSocketSet;
	std::vector<std::thread*>   m_socketThread;
	std::vector<TCPSocketInfo>  m_socketInfoVec;
	std::vector<WorkThreadInfo> m_workBaseVec;

#ifdef __WebSocket__
	SSL_CTX*	m_ctx;
	X509*		m_cert;
#endif // __WebSocket__
};