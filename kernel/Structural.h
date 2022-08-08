#pragma once

const int					HD_SOCKET_READ = 1;									// SOCKET ��ȡ�¼�����			
const int					HD_SOCKET_CLOSE = 2;								// SOCKET �ر��¼�����			
const int					HD_ASYN_THREAD_RESULT = 3;							// �첽�߳̽������
const int					HD_TIMER_MESSAGE = 4;								// ��ʱ����Ϣ����
const int					HD_PLATFORM_SOCKET_READ = 5;						// ���ķ���ȡ�¼�����

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

// ��־�ȼ�
enum LogLevel
{
	LOG_INFO = 0,																//info
	LOG_WARN,																	//warnning
	LOG_ERROR,																	//error
	LOG_CINFO,																	//info Console tip
	LOG_CERROR,																	//error Console
	LOG_ERROR_SYS,																//error system
	LOG_END,
};

// ����������
enum class ServiceType
{
	SERVICE_TYPE_BEGIN          = 0,
	SERVICE_TYPE_LOGIC          = 1,											// �߼�������
	SERVICE_TYPE_DB             = 2,											// DB������
	SERVICE_TYPE_GATA           = 3,											// ���ط�����
	SERVICE_TYPE_GAMECENTER     = 4,											// GameCenter
	SERVICE_TYPE_LOGON          = 5,											// LOGON
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

/**
* @brief �����ֶ����ͣ�
* DB_INT:��������
* DB_STR:�ַ�������
*/
enum class FT
{
	DB_INT,
	DB_STR,
};

#pragma pack(1)

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
	UINT uMainID;																//��ʱ�� ID
	UINT uTimerID;																//��ʱ�� ID
	ServerTimerLine() { memset(this, 0, sizeof(ServerTimerLine)); }
};

//�������ݰ��ṹͷ
struct NetMessageHead
{
	unsigned int uMainID;														//����������
	unsigned int uAssistantID;													//������������ ID
	unsigned int uHandleCode;													//���ݰ���������
	unsigned int uIdentification;												//���ݱ�ʶ����ͬ��Э�������в�ͬ�ĺ��壩
	unsigned int uMessageSize;													//���ݰ���С

	NetMessageHead() { memset(this, 0, sizeof(NetMessageHead)); }
};

// �����߳���Ϣ
struct WorkThreadInfo
{
	struct event_base*	base;
	struct event*		event;													//read_fd�Ķ��¼�
	SOCKET				read_fd;
	SOCKET				write_fd;

	WorkThreadInfo() { memset(this, 0, sizeof(WorkThreadInfo)); }
};

// TCP��Ϣ
struct TCPSocketInfo
{
	bool			isConnect;
	time_t			acceptMsgTime;
	char			ip[MAX_NUM_IP_ADDR_SIZE];
	unsigned short	port;
	SOCKET			acceptFd;													//�Լ���socket
	bufferevent*	bev;
	std::mutex*		lock;
	bool			bHandleAccptMsg;											//�Ƿ�����������Ϣ��websocketʹ��

	TCPSocketInfo() { memset(this, 0, sizeof(TCPSocketInfo)); }
};

// Ͷ��soket
struct PlatformSocketInfo
{
	time_t			acceptMsgTime;
	char			ip[MAX_NUM_IP_ADDR_SIZE];
	unsigned short	port;
	SOCKET			acceptFd;													//�Լ���socket

	PlatformSocketInfo() { memset(this, 0, sizeof(PlatformSocketInfo)); }
};

// �������ݶ��а�ͷ
struct SendDataLineHead
{
	DataLineHead			dataLineHead;										//����ͷ
	int						socketIndex;										//socket���������ļ�������
	void*					pBufferevent;										//bufferevent	

	SendDataLineHead() { memset(this, 0, sizeof(SendDataLineHead)); }
};

// SOCKET��ȡ֪ͨ�ṹ����
struct SocketReadLine
{
	DataLineHead						LineHead;								//����ͷ
	NetMessageHead						netMessageHead;							//���ݰ�ͷ
	unsigned int						uHandleSize;							//���ݰ�������С
	unsigned int						uIndex;									//SOCKET ����
	unsigned long int					uAccessIP;								//SOCKET IP
	void*								pBufferevent;							//bufferevent
	SocketType							socketType;								//socket���� enum SocketType

	SocketReadLine() { memset(this, 0, sizeof(SocketReadLine)); }
};

//SOCKET�ر�֪ͨ�ṹ����
struct SocketCloseLine
{
	DataLineHead						LineHead;								//����ͷ
	UINT								uIndex;									//SOCKT ����
	ULONG								uAccessIP;								//SOCKET IP
	UINT								uConnectTime;							//����ʱ��
	BYTE								socketType;								//socket���� enum SocketType

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
	BYTE			timertype;													// ��ʱ������ SERVERTIMER_TYPE

	ServerTimerInfo() : elapse(10), starttime(0), timertype(SERVERTIMER_TYPE_PERISIST) {}
};

// �����Ϣ
struct PlayerInfo
{
	SocketReadLine*			pMsg;					// SOCKET��ȡ֪ͨ�ṹ����
	void*					pData;				// ��ҷ��͹���������
	ServiceType				uSrverType;			// ����������
	uint64_t				userId;				// ���id

	PlayerInfo() : pMsg(nullptr), pData(nullptr), uSrverType(ServiceType::SERVICE_TYPE_LOGIC), userId(0) {}
	~PlayerInfo() {}
};

// ������Ҽ�
struct LoadPlayerKey
{
	unsigned int			index;
	uint64_t				userId;
	std::string				id;
	std::string				pw;

	LoadPlayerKey(int nIndex, std::string& sId, std::string& sPw) : index(nIndex), userId(0), id(sId), pw(sPw) {}
	~LoadPlayerKey() {}
};

//�����̲߳���
class CTCPSocketManage;
struct RecvThreadParam
{
	CTCPSocketManage*		pThis;
	int						index;

	RecvThreadParam() :pThis(nullptr), index(0) {}
};

class PlayerClient;
using TimerList				= std::list<UINT>;
using AttrsMap				= std::map<int, int>;
using OnLinePlayerSet		= std::set<unsigned int>;
using SqlList				= std::list<std::string>;
using LoadPlayerList		= std::list<LoadPlayerKey>;
using PlayerClientVec		= std::vector<PlayerClient*>;
using SqlKeyDataMap			= std::map<std::string, std::string>;
using AttrsFunMap			= std::vector<std::function<void()>>;
using TimerFunMap			= std::map<TimerCmd, std::function<void()>>;
using NetFunMap				= std::map<MsgCmd, std::function<void(PlayerInfo*)>>;
using ExitFunMap			= std::vector<std::function<void(SocketCloseLine*)>>;
using MysqlFunMap			= std::map<std::string, std::function<void(std::string&)>>;
using TypeFunMap			= std::map<unsigned int, std::function<void(void* pDataLineHead)>>;
using ServerTimerInfomap	= std::unordered_map<unsigned int, ServerTimerInfo>;
using RecordDataMap			= std::map<std::string, std::pair<FT, std::string>>;
using LogLevelNames			= const std::array<const char*, LOG_END>;

LogLevelNames levelNames = { "[INF]", "[WAR]", "[ERR]", "[INF]","[ERR]", "[SYS]", };