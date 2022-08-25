#pragma once
#include <string.h>
#include <mutex>
#include <set>
#include <map>
#include <list>
#include <vector>
#include <thread>
#include <unordered_map>
#include <functional>

#ifdef _WIN32
//define something for Windows (32-bit and 64-bit, this part is common)
#ifdef _WIN64
   //define something for Windows (64-bit only)
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define EXPORT_DLL __declspec(dllexport) //导出dll声明
#define FD SOCKET
#else
   //define something for Windows (32-bit only)
#endif
#elif __APPLE__
#if TARGET_IPHONE_SIMULATOR
// iOS Simulator
#elif TARGET_OS_IPHONE
// iOS device
#elif TARGET_OS_MAC
// Other kinds of Mac OS
#elif __ANDROID__
// android
#elif __linux__
// linux
#define EXPORT_DLL 
#define FD int
#elif __unix__ // all unices not caught above
// Unix
#elif defined(_POSIX_VERSION)
// POSIX
#else
#   error "Unknown"
#endif
#endif

const int					CREATE_TABLE_LEN = 512;								// 创建数据库语句长度
const int					LOG_BUF_LEN = 20 * 1024;							// 日志打印缓存
const int					MAX_TIMER_THRED_NUMS = 4;							// 定时器最大线程数量
const int					MIN_TIMER_THRED_NUMS = 1;							// 定时器最小线程数量

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
const unsigned int			MAX_DATALINE_LEN = 320000;

const int					MAX_LOG_BUF_SIZE = 1024;							// log缓冲区大小
const int					MAX_FILE_NAME_SIZE = 256;							// 最大文件名长度
const int					MAX_FUNC_NAME_SIZE = 128;							// 最大函数名长度
const int					MAX_LOG_FILE_SIZE = 1024 * 1024 * 30;				//日子文件的最大的长度，超过长度会生成新的文件

enum class SysMsgCmd
{
	HD_SYS_MSG_BEGIN			= 0,											// 开始
	HD_SOCKET_READ				= 1,											// FD 读取事件处理
	HD_SOCKET_CLOSE				= 2,											// FD 关闭事件处理
	HD_ASYN_THREAD_RESULT		= 3,											// 异步线程结果处理
	HD_TIMER_MESSAGE			= 4,											// 定时器消息处理
	HD_PLATFORM_SOCKET_READ		= 5,											// 中心服读取事件处理
};

// 服务器类型
enum class ServiceType
{
	SERVICE_TYPE_BEGIN          = 0,
	SERVICE_TYPE_LOGIC          = 1,											// 逻辑服务器
	SERVICE_TYPE_DB             = 2,											// DB服务器
	SERVICE_TYPE_GATA           = 3,											// 网关服务器
	SERVICE_TYPE_GAMECENTER     = 4,											// GameCenter
	SERVICE_TYPE_LOGON			= 5,											// LOGON
	SERVICE_TYPE_CLIENT         = 6,											// Client
	SERVICE_TYPE_END,
};

// socket类型
enum class SocketType
{
	SOCKET_TYPE_TCP             = 0,											//tcp
	SOCKET_TYPE_WEBSOCKET       = 1,											//websocket
};

// 定时器类型
enum SERVERTIMER_TYPE
{
	SERVERTIMER_TYPE_PERISIST   = 0,											// 持久定时器
	SERVERTIMER_TYPE_SINGLE     = 1,											// 一次性定时器
};

// 场景内类型
enum class AnimalType
{
	at_player		= 1,														// 玩家
	at_monster		= 2,														// 怪物
};

// 场景内类型
enum class HeroType
{
	ht_shooter		= 1,														// 射手
	ht_warrior		= 2,														// 战士
	ht_master		= 3,														// 法师
	ht_meat			= 4,														// 肉
	ht_wild			= 5,														// 打野
};

class Animal;
class PlayerClient;
enum class ActType;
struct ActtiveOpen;
struct CActivityList;
class CTCPSocketManage;
enum class HurtSysMsgCmd;

struct REvent
{
	char m_Source[2048];
	REvent() { memset(this, 0, sizeof(REvent)); }
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
	unsigned int uMainID;																//定时器 ID
	unsigned int uTimerID;																//定时器 ID
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
	FD				read_fd;
	FD				write_fd;

	WorkThreadInfo() { memset(this, 0, sizeof(WorkThreadInfo)); }
};

// TCP信息
struct TCPSocketInfo
{
	bool			isConnect;
	bufferevent*	bev;
	std::mutex*		lock;

	TCPSocketInfo() { memset(this, 0, sizeof(TCPSocketInfo)); }
};

// 投递soket
struct PlatformSocketInfo
{
	time_t			acceptMsgTime;
	char			ip[MAX_NUM_IP_ADDR_SIZE];
	unsigned short	port;
	FD			acceptFd;													//自己的socket

	PlatformSocketInfo() { memset(this, 0, sizeof(PlatformSocketInfo)); }
};

// 发送数据队列包头
struct SendDataLineHead
{
	DataLineHead			dataLineHead;										//队列头
	void*					pBufferevent;										//bufferevent	

	SendDataLineHead() { memset(this, 0, sizeof(SendDataLineHead)); }
};

// FD读取通知结构定义
struct SocketReadLine
{
	DataLineHead						LineHead;								//队列头
	NetMessageHead						netMessageHead;							//数据包头
	unsigned int						uHandleSize;							//数据包处理大小
	void*								pBufferevent;							//bufferevent

	SocketReadLine() { memset(this, 0, sizeof(SocketReadLine)); }
};

//FD关闭通知结构定义
struct SocketCloseLine
{
	DataLineHead								LineHead;								//队列头
	unsigned int								uIndex;									//SOCKT 索引
	unsigned long								uAccessIP;								//FD IP
	unsigned int								uConnectTime;							//连接时间
	unsigned char								socketType;								//socket类型 enum SocketType

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
	DataLineHead		stDataHead;
	unsigned char*		pData;

	ListItemData() : pData(nullptr) {}
};

// 定时器结构
struct ServerTimerInfo
{
	unsigned int	elapse;														// 定时器间隔（单位毫秒）
	long long		starttime;													// 起始时间（单位毫秒）
	unsigned char	timertype;													// 定时器类型 SERVERTIMER_TYPE

	ServerTimerInfo() : elapse(10), starttime(0), timertype(SERVERTIMER_TYPE_PERISIST) {}
};

typedef void (*NetworkCallBackFunc)(REvent eve/*, char m_Source[]*/);
typedef void (*TimerCallBackFunc)(int timer);

template<typename T>
using CfgSet				= std::set<T>;
template<typename T>
using CfgVector				= std::vector<T>;
template<typename T>
using CfgMap				= std::map<int, CfgVector<T>>;
using IntVector				= std::vector<int>;
using TimerList				= std::list<unsigned int>;
using OnLinePlayerSet		= std::set<unsigned int>;
using SqlList				= std::list<std::string>;
using PlayerClientVec		= std::vector<PlayerClient*>;
using SqlKeyDataMap			= std::map<std::string, std::string>;
using AttrsFunMap			= std::vector<std::function<void()>>;
using ExitFunMap			= std::vector<std::function<void(SocketCloseLine*)>>;
using MysqlFunMap			= std::map<std::string, std::function<void(std::string&)>>;
using TypeFunMap			= std::map<SysMsgCmd, std::function<void(void* pDataLineHead)>>;
using ServerTimerInfomap	= std::unordered_map<unsigned int, ServerTimerInfo>;
using ActivityFunMap		= std::map<ActType, std::function<bool(CActivityList*)>>;
using SceneAnimalMap		= std::map<int, std::map<uint64_t, Animal*>>;
using ActtiveOpenMap		= std::map<int, ActtiveOpen>;
using TimerFunMap			= std::map<int, TimerCallBackFunc>;

