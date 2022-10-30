#pragma once

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

enum WS_FrameType
{
	WS_EMPTY_FRAME = 0xF0,
	WS_ERROR_FRAME = 0xF1,
	WS_TEXT_FRAME = 0x01,
	WS_BINARY_FRAME = 0x02,
	WS_PING_FRAME = 0x09,
	WS_PONG_FRAME = 0x0A,
	WS_OPENING_FRAME = 0xF3,
	WS_CLOSING_FRAME = 0x08
};

enum class SysMsgCmd
{
	HD_SYS_MSG_BEGIN			= 0,											// ��ʼ
	HD_SOCKET_READ				= 1,											// SOCKFD ��ȡ�¼�����
	HD_SOCKET_CLOSE				= 2,											// SOCKFD �ر��¼�����
	HD_ASYN_THREAD_RESULT		= 3,											// �첽�߳̽������
	HD_TIMER_MESSAGE			= 4,											// ��ʱ����Ϣ����
	HD_PLATFORM_SOCKET_READ		= 5,											// ���ķ���ȡ�¼�����
};

// ��־�ȼ�
enum LogLevel
{
	INF = 0,																//info
	WAR,																	//warnning
	ERR,																	//error
	CINF,																	//info Console tip
	CERR,																	//error Console
	SYS,																	//error system
	LOG_END,
};

// ����������
enum class ServiceType
{
	SERVICE_TYPE_BEGIN          = 0,
	SERVICE_TYPE_LOGIC          = 1,											// �߼�������
	SERVICE_TYPE_DB             = 2,											// DB������
	SERVICE_TYPE_GATA           = 3,											// ���ط�����
	SERVICE_TYPE_CROSS			= 4,											// GameCenter
	SERVICE_TYPE_LOGON          = 5,											// LOGON
	SERVICE_TYPE_LOGIC_WS		= 6,											// WS�߼�������
	SERVICE_TYPE_LOGIC_WSS		= 7,											// WSS�߼�������
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

// ����������
enum class AnimalType
{
	at_player		= 1,														// ���
	at_monster		= 2,														// ����
};

// ����������
enum class HeroType
{
	ht_shooter		= 1,														// ����
	ht_warrior		= 2,														// սʿ
	ht_master		= 3,														// ��ʦ
	ht_meat			= 4,														// ��
	ht_wild			= 5,														// ��Ұ
};

class Animal;
class PlayerClient;
enum class ActType;
struct ActtiveOpen;
struct CActivityList;
class CTCPSocketManage;
enum class HurtSysMsgCmd;

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
	SOCKFD				read_fd;
	SOCKFD				write_fd;

	WorkThreadInfo() { memset(this, 0, sizeof(WorkThreadInfo)); }
};

// TCP��Ϣ
struct TCPSocketInfo
{
	bool			isConnect;
	time_t			acceptMsgTime;
	char			ip[MAX_NUM_IP_ADDR_SIZE];
	unsigned short	port;
	SOCKFD			acceptFd;													//�Լ���socket
	bufferevent*	bev;
	std::mutex*		lock;
	bool			bHandleAccptMsg;											//�Ƿ�����������Ϣ��websocketʹ��
	uint64_t		link;														//���������Ƿ�ɹ�
	SSL*			ssl;
	bool			isCross;													//�Ƿ��ǿ��true���false����

	void Reset(ServiceType& serviceType);

	TCPSocketInfo() { memset(this, 0, sizeof(TCPSocketInfo)); }
};

// Ͷ��soket
struct PlatformSocketInfo
{
	time_t			acceptMsgTime;
	char			ip[MAX_NUM_IP_ADDR_SIZE];
	unsigned short	port;
	SOCKFD			acceptFd;													//�Լ���socket

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

// SOCKFD��ȡ֪ͨ�ṹ����
struct SocketReadLine
{
	DataLineHead						LineHead;								//����ͷ
	NetMessageHead						netMessageHead;							//���ݰ�ͷ
	unsigned int						uHandleSize;							//���ݰ������С
	unsigned int						uIndex;									//SOCKFD ����
	unsigned long int					uAccessIP;								//SOCKFD IP
	void*								pBufferevent;							//bufferevent
	SocketType							socketType;								//socket���� enum SocketType

	SocketReadLine() { memset(this, 0, sizeof(SocketReadLine)); }
};

//SOCKFD�ر�֪ͨ�ṹ����
struct SocketCloseLine
{
	DataLineHead						LineHead;								//����ͷ
	unsigned int						uIndex;									//SOCKT ����
	unsigned long						uAccessIP;								//SOCKFD IP
	unsigned int						uConnectTime;							//����ʱ��
	bool								isCross;								//�Ƿ��ڿ��
	uint64_t							userid;									//���id

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

// �����Ϣ
struct PlayerInfo
{
	SocketReadLine*			pMsg;				// SOCKFD��ȡ֪ͨ�ṹ����
	void*					pData;				// ��ҷ��͹���������
	ServiceType				uSrverType;			// ����������
	uint64_t				userId;				// ���id

	PlayerInfo() : pMsg(nullptr), pData(nullptr), uSrverType(ServiceType::SERVICE_TYPE_LOGIC), userId(0) {}
	~PlayerInfo() {}
};

struct LoginData
{
	unsigned int			index;
	std::string				id;
	std::string				pw;
	std::string				roleName;	// Ӣ����
	std::string				netName;	// ��Ϸ��
	uint64_t				userId;
	int						roleid;
	int						roleType;
	int						serverId;
	int						logicIndex;

	LoginData() : index(0), id(""), pw(""), roleid(0), userId(0), roleName(""), 
		netName(""), roleType(0), serverId(0), logicIndex(-1) {}
	~LoginData() {}
};

//�����̲߳���
struct RecvThreadParam
{
	CTCPSocketManage*		pThis;
	int						index;

	RecvThreadParam() :pThis(nullptr), index(0) {}
};

struct Position
{
	int x;
	int y;
	int z;
	Position() : x(0), y(0), z(0) {}
	~Position() {}
};

struct Rotation
{
	int x;
	int y;
	int z;
	Rotation() : x(0), y(0), z(0) {}
	~Rotation() {}
};

struct Scale
{
	int x;
	int y;
	int z;
	Scale() : x(0), y(0), z(0) {}
	~Scale() {}
};

struct Transform
{
	Position	position;
	Rotation	rotation;
	Scale		scale;

	Transform() {}
	Transform(int x, int y)
	{
		position.x = x;
		position.y = y;
	}

	~Transform() {}
};

struct RefMonsterKey
{
	int mid;
	int x;
	int y;
	~RefMonsterKey(){}
	RefMonsterKey() : mid(0), x(0), y(0) {}
	RefMonsterKey(int mId, int X, int Y) : mid(mId), x(X), y(Y) {}
	bool operator < (const RefMonsterKey& other) const
	{
		if (mid != other.mid) { return mid < other.mid; }
		if (x != other.x) { return x < other.x; }
		if (y != other.y) { return y < other.y; }
		return false;
	}
};

struct ActtiveOpen
{
	int id;
	bool open;
	ActtiveOpen() : id(0), open(false) {}
	ActtiveOpen(int nId, bool isOpen) : id(nId), open(isOpen) {}
	~ActtiveOpen() {}
};

struct SkillData
{
	bool cd;
	int level;
};

struct SkillCDData
{
	HurtSysMsgCmd type;
	int id;
	int cnt;
	Animal* animal;

	SkillCDData(HurtSysMsgCmd cmd, int nid, int ncnt, Animal* ani) : type(cmd), id(nid), cnt(ncnt), animal(ani) {}
	~SkillCDData() {}
};

#ifdef __WebSocket__
struct WebSocketMsg
{
	uint8_t fin;
	uint8_t opcode;
	uint8_t mask;
	uint8_t maskingKey[4];
	uint32_t dataLength;	//���ݰ��ܳ���
	uint32_t payloadLength; //���ݲ��ֳ��ȣ���������ͷ
	char* payload;			//���ݲ�����ʼ��ַ

	WebSocketMsg()
	{
		Init();
	}

	void Init()
	{
		fin = 0;
		opcode = 0;
		mask = 0;
		maskingKey[0] = 0;
		dataLength = 0;
		payloadLength = 0;
		payload = nullptr;
	}
};
#endif

struct SLoadMysql
{
	int serverid;
	std::string sqlName;
	unsigned int uMainID;
	unsigned int uAssistantID;
	unsigned int uIdentification;
	unsigned int uIndex;

	SLoadMysql(unsigned int index, std::string sqlname, MsgCmd mainID,
		unsigned int assistantID, MsgCmd identification = MsgCmd::MsgCmd_Player, int sid = 0);
	~SLoadMysql() {}
};