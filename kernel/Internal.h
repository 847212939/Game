#pragma once

//处理类型定义
#define HD_SOCKET_READ					1										// SOCKET 读取事件处理			
#define HD_SOCKET_CLOSE					2										// SOCKET 关闭事件处理			
#define HD_ASYN_THREAD_RESULT			3										// 异步线程结果处理
#define HD_TIMER_MESSAGE				4										// 定时器消息处理
#define HD_PLATFORM_SOCKET_READ			5										// 中心服读取事件处理
#define CREATE_TABLE_LEN				512										// 创建数据库语句长度
#define LOG_BUF_LEN						20 * 1024								// 日志打印缓存
#define MAX_TIMER_THRED_NUMS			4										// 定时器最大线程数量
#define MIN_TIMER_THRED_NUMS			1										// 定时器最小线程数量

//通用变量的定义
const unsigned int			MAX_NUM_IP_ADDR_SIZE = 48;							// 最大数字IP地址长度
const static int			ERROR_SERVICE_FULL = 15;							// 服务器人数已满
const int					MAX_POST_CONNECTED_COUNT = 128;						// 连接线程投递到接收线程socket最大数量
const int					MAX_TEMP_SENDBUF_SIZE = 8 * 1024;					// 临时发送缓冲区大小，也是单条数据包的最大长度，可以改成16K
const int					SOCKET_SEND_BUF_SIZE = MAX_TEMP_SENDBUF_SIZE * 8;	// TCP发送缓冲区最小值
const int					SOCKET_RECV_BUF_SIZE = MAX_TEMP_SENDBUF_SIZE * 8;	// TCP接收缓冲区最小值

// 心跳、socket相关
const unsigned int			CHECK_HEAETBEAT_SECS = 15;							// 心跳定时器时间(s)
const unsigned int			KEEP_ACTIVE_HEARTBEAT_COUNT = 3;					// 前端和服务器的心跳

// 消息队列最大长度
const unsigned int MAX_DATALINE_LEN = 320000;

// 服务器类型
enum class ServiceType
{
	SERVICE_TYPE_BEGIN          = 0,
	SERVICE_TYPE_LOGIC          = 1,											// 逻辑服务器
	SERVICE_TYPE_DB             = 2,											// DB服务器
	SERVICE_TYPE_GATA           = 3,											// 网关服务器
	SERVICE_TYPE_GAMECENTER     = 4,											// GameCenter
	SERVICE_TYPE_LOGON          = 5,											// LOGON
	SERVICE_TYPE_END,
};

// socket类型
enum class SocketType
{
	SOCKET_TYPE_TCP             = 0,											//tcp
	SOCKET_TYPE_WEBSOCKET       = 1,											//websocket
};

enum SERVERTIMER_TYPE
{
	SERVERTIMER_TYPE_PERISIST   = 0,											// 持久定时器
	SERVERTIMER_TYPE_SINGLE     = 1,											// 一次性定时器
};

#pragma pack(1)

//数据队列信息头
struct DataLineHead
{
	unsigned int uSize;															//数据大小
	unsigned int uDataKind;														//数据类型
	DataLineHead() { memset(this, 0, sizeof(DataLineHead)); }	
};

//定时器消息结构定义
struct ServerTimerLine
{
	UINT uMainID;																//定时器 ID
	UINT uTimerID;																//定时器 ID
	ServerTimerLine() { memset(this, 0, sizeof(ServerTimerLine)); }
};

//网络数据包结构头
struct NetMessageHead
{
	unsigned int uMainID;														//处理主类型
	unsigned int uAssistantID;													//辅助处理类型 ID
	unsigned int uHandleCode;													//数据包处理代码
	unsigned int uIdentification;												//身份标识（不同的协议里面有不同的含义）
	unsigned int uMessageSize;													//数据包大小
	NetMessageHead() { memset(this, 0, sizeof(NetMessageHead)); }
};

// 工作线程信息
struct WorkThreadInfo
{
	struct event_base*	base;
	struct event*		event;													//read_fd的读事件
	SOCKET				read_fd;
	SOCKET				write_fd;
	WorkThreadInfo() { memset(this, 0, sizeof(WorkThreadInfo)); }
};

// TCP信息
struct TCPSocketInfo
{
	bool			isConnect;
	time_t			acceptMsgTime;
	char			ip[MAX_NUM_IP_ADDR_SIZE];
	unsigned short	port;
	SOCKET			acceptFd;													//自己的socket
	bufferevent*	bev;
	std::mutex*		lock;
	bool			bHandleAccptMsg;											//是否处理了握手消息，websocket使用
	TCPSocketInfo() { memset(this, 0, sizeof(TCPSocketInfo)); }
};

// 投递soket
struct PlatformSocketInfo
{
	time_t			acceptMsgTime;
	char			ip[MAX_NUM_IP_ADDR_SIZE];
	unsigned short	port;
	SOCKET			acceptFd;													//自己的socket
	PlatformSocketInfo() { memset(this, 0, sizeof(PlatformSocketInfo)); }
};

// 发送数据队列包头
struct SendDataLineHead
{
	DataLineHead			dataLineHead;										//队列头
	int						socketIndex;										//socket索引或者文件描述符
	void*					pBufferevent;										//bufferevent		
	SendDataLineHead() { memset(this, 0, sizeof(SendDataLineHead)); }
};

// SOCKET读取通知结构定义
struct SocketReadLine
{
	DataLineHead						LineHead;								//队列头
	NetMessageHead						netMessageHead;							//数据包头
	unsigned int						uHandleSize;							//数据包处理大小
	unsigned int						uIndex;									//SOCKET 索引
	unsigned long int					uAccessIP;								//SOCKET IP
	void*								pBufferevent;							//bufferevent
	SocketType							socketType;								//socket类型 enum SocketType
	SocketReadLine() { memset(this, 0, sizeof(SocketReadLine)); }
};

//SOCKET关闭通知结构定义
struct SocketCloseLine
{
	DataLineHead						LineHead;								//队列头
	UINT								uIndex;									//SOCKT 索引
	ULONG								uAccessIP;								//SOCKET IP
	UINT								uConnectTime;							//连接时间
	BYTE								socketType;								//socket类型 enum SocketType
	SocketCloseLine() { memset(this, 0, sizeof(SocketCloseLine)); }
};

#pragma pack()

/*
Struct		:ListItemData
Memo		:链表项数据结构
Parameter	:
stDataHead	:数据包头
pData		:每个链表项数据的指针，使用用new方式申请的内存，注意，在出队列时，要显式的delete 该内存
*/
struct ListItemData
{
	DataLineHead stDataHead;
	unsigned char* pData;
	ListItemData() : pData(nullptr) {}
};

struct ServerTimerInfo
{
	unsigned int elapse;														// 定时器间隔（单位毫秒）
	long long starttime;														// 起始时间（单位毫秒）
	BYTE timertype;																// 定时器类型 SERVERTIMER_TYPE
	ServerTimerInfo()
	{
		elapse = 10;
		starttime = 0;
		timertype = SERVERTIMER_TYPE_PERISIST;
	}
};
