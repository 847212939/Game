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

#if defined(_WIN32)
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define EXPORT_DLL __declspec(dllexport) //����dll����
#define EXPORT_STDCALL __stdcall
#define SockFd SOCKET
#elif defined(_WIN64)
#elif defined(__linux__)
#define EXPORT_DLL
#define EXPORT_STDCALL
#define SockFd int
#elif defined(__unix__)
#elif defined(__ANDROID__)
#elif defined(__APPLE__)
#endif

const int					CREATE_TABLE_LEN = 512;								// �������ݿ���䳤��
const int					LOG_BUF_LEN = 20 * 1024;							// ��־��ӡ����
const int					MAX_TIMER_THRED_NUMS = 4;							// ��ʱ������߳�����
const int					MIN_TIMER_THRED_NUMS = 1;							// ��ʱ����С�߳�����

const unsigned int			MAX_NUM_IP_ADDR_SIZE = 48;							// �������IP��ַ����
const static int			ERROR_SERVICE_FULL = 15;							// ��������������
const int					MAX_POST_CONNECTED_COUNT = 128;						// �����߳�Ͷ�ݵ������߳�socket�������
const int					MAX_TEMP_SENDBUF_SIZE = 8 * 1024;					// ��ʱ���ͻ�������С��Ҳ�ǵ������ݰ�����󳤶ȣ����Ըĳ�16K
const int					SOCKET_SEND_BUF_SIZE = MAX_TEMP_SENDBUF_SIZE * 8;	// TCP���ͻ�������Сֵ
const int					SOCKET_RECV_BUF_SIZE = MAX_TEMP_SENDBUF_SIZE * 8;	// TCP���ջ�������Сֵ

// ������socket���
const unsigned int			CHECK_HEAETBEAT_SECS = 15;							// ������ʱ��ʱ��(s)
const unsigned int			KEEP_ACTIVE_HEARTBEAT_COUNT = 3;					// ǰ�˺ͷ�����������

// ��Ϣ������󳤶�
const unsigned int			MAX_DATALINE_LEN = 320000;

const int					MAX_LOG_BUF_SIZE = 1024;							// log��������С
const int					MAX_FILE_NAME_SIZE = 256;							// ����ļ�������
const int					MAX_FUNC_NAME_SIZE = 128;							// �����������
const int					MAX_LOG_FILE_SIZE = 1024 * 1024 * 30;				//�����ļ������ĳ��ȣ��������Ȼ������µ��ļ�

enum class SysMsgCmd
{
	HD_SYS_MSG_BEGIN			= 0,											// ��ʼ
	HD_SOCKET_READ				= 1,											// SockFd ��ȡ�¼�����
	HD_SOCKET_CLOSE				= 2,											// SockFd �ر��¼�����
	HD_ASYN_THREAD_RESULT		= 3,											// �첽�߳̽������
	HD_TIMER_MESSAGE			= 4,											// ��ʱ����Ϣ����
	HD_PLATFORM_SOCKET_READ		= 5,											// ���ķ���ȡ�¼�����
};

// ����������
enum class ServiceType
{
	SERVICE_TYPE_BEGIN				= 0,
	SERVICE_TYPE_LOGIC				= 1,											// �߼�������
	SERVICE_TYPE_DB					= 2,											// DB������
	SERVICE_TYPE_GATA				= 3,											// ���ط�����
	SERVICE_TYPE_CROSS				= 4,											// Cross
	SERVICE_TYPE_LOGON				= 5,											// LOGON
	SERVICE_TYPE_CLIENT				= 6,											// Client
	SERVICE_TYPE_CLIENT_HEARTBEAT   = 7,											// ClientHeartbeat
	SERVICE_TYPE_END,
};

// socket����
enum class SocketType
{
	SOCKET_TYPE_TCP             = 0,											//tcp
	SOCKET_TYPE_WEBSOCKET       = 1,											//websocket
};

// ��ʱ������
enum SERVERTIMER_TYPE
{
	SERVERTIMER_TYPE_PERISIST   = 0,											// �־ö�ʱ��
	SERVERTIMER_TYPE_SINGLE     = 1,											// һ���Զ�ʱ��
};

class Animal;
class PlayerClient;
enum class ActType;
struct ActtiveOpen;
struct CActivityList;
class CTCPSocketManage;
enum class HurtSysMsgCmd;


#pragma pack(1)

struct REvent
{
	char m_Source[2048];
	REvent() { memset(this, 0, sizeof(REvent)); }
};

//���ݶ�����Ϣͷ
struct DataLineHead
{
	unsigned int uSize;															//���ݴ�С
	unsigned int uDataKind;														//��������

	DataLineHead() { memset(this, 0, sizeof(DataLineHead)); }	
};

//��ʱ����Ϣ�ṹ����
struct ServerTimerLine
{
	unsigned int uMainID;																//��ʱ�� ID
	unsigned int uTimerID;																//��ʱ�� ID
	ServerTimerLine() { memset(this, 0, sizeof(ServerTimerLine)); }
};

//�������ݰ��ṹͷ
struct NetMessageHead
{
	unsigned int uMainID;														//����������
	unsigned int uAssistantID;													//������������ ID
	unsigned int uHandleCode;													//���ݰ��������
	unsigned int uIdentification;												//��ݱ�ʶ����ͬ��Э�������в�ͬ�ĺ��壩
	unsigned int uMessageSize;													//���ݰ���С

	NetMessageHead() { memset(this, 0, sizeof(NetMessageHead)); }
};

// �����߳���Ϣ
struct WorkThreadInfo
{
	struct event_base*	base;
	struct event*		event;													//read_fd�Ķ��¼�
	SockFd				read_fd;
	SockFd				write_fd;

	WorkThreadInfo() { memset(this, 0, sizeof(WorkThreadInfo)); }
};

// �������ݶ��а�ͷ
struct SendDataLineHead
{
	DataLineHead			dataLineHead;										//����ͷ

	SendDataLineHead() { memset(this, 0, sizeof(SendDataLineHead)); }
};

// SockFd��ȡ֪ͨ�ṹ����
struct SocketReadLine
{
	DataLineHead						LineHead;								//����ͷ
	NetMessageHead						netMessageHead;							//���ݰ�ͷ
	unsigned int						uHandleSize;							//���ݰ������С

	SocketReadLine() { memset(this, 0, sizeof(SocketReadLine)); }
};

//SockFd�ر�֪ͨ�ṹ����
struct SocketCloseLine
{
	unsigned int								uIndex;									//SOCKT ����
	SocketCloseLine() { memset(this, 0, sizeof(SocketCloseLine)); }
};

#pragma pack()

/*
Struct		:ListItemData
Memo		:���������ݽṹ
Parameter	:
stDataHead	:���ݰ�ͷ
pData		:ÿ�����������ݵ�ָ�룬ʹ����new��ʽ������ڴ棬ע�⣬�ڳ�����ʱ��Ҫ��ʽ��delete ���ڴ�
*/
struct ListItemData
{
	DataLineHead		stDataHead;
	unsigned char*		pData;

	ListItemData() : pData(nullptr) {}
};

// ��ʱ���ṹ
struct ServerTimerInfo
{
	unsigned int	elapse;														// ��ʱ���������λ���룩
	long long		starttime;													// ��ʼʱ�䣨��λ���룩
	unsigned char	timertype;													// ��ʱ������ SERVERTIMER_TYPE

	ServerTimerInfo() : elapse(10), starttime(0), timertype(SERVERTIMER_TYPE_PERISIST) {}
};

typedef void (*NetworkCallBackFunc)(REvent eve/*, char m_Source[]*/);
typedef void (*TimerCallBackFunc)(int timer);
typedef void (*CloseCallBackFunc)();

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