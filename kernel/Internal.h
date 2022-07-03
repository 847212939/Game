#pragma once

//�������Ͷ���
#define HD_SOCKET_READ					1							//SOCKET ��ȡ�¼�����			
#define HD_SOCKET_CLOSE					2							//SOCKET �ر��¼�����			
#define HD_ASYN_THREAD_RESULT			3							//�첽�߳̽������
#define HD_TIMER_MESSAGE				4							//��ʱ����Ϣ����
#define HD_PLATFORM_SOCKET_READ			5							//���ķ���ȡ�¼�����

// ����������
enum class ServiceType
{
	SERVICE_TYPE_BEGIN = 0,
	SERVICE_TYPE_LOGIC,		// �߼�������
	SERVICE_TYPE_DB,		// DB������
	SERVICE_TYPE_GATA,		// ���ط�����
	SERVICE_TYPE_GAMECENTER,// GameCenter
	SERVICE_TYPE_LOGON,		// LOGON
	SERVICE_TYPE_END,
};

// socket����
enum class SocketType
{
	SOCKET_TYPE_TCP = 0,				//tcp
	SOCKET_TYPE_WEBSOCKET = 1,			//websocket
};

//ͨ�ñ����Ķ���
const unsigned int			MAX_NUM_IP_ADDR_SIZE = 48;							// �������IP��ַ����
const static int			ERROR_SERVICE_FULL = 15;							// ��������������
const int					MAX_POST_CONNECTED_COUNT = 128;						// �����߳�Ͷ�ݵ������߳�socket�������
const int					MAX_TEMP_SENDBUF_SIZE = 8 * 1024;					// ��ʱ���ͻ�������С��Ҳ�ǵ������ݰ�����󳤶ȣ����Ըĳ�16K
const int					SOCKET_SEND_BUF_SIZE = MAX_TEMP_SENDBUF_SIZE * 8;	// TCP���ͻ�������Сֵ
const int					SOCKET_RECV_BUF_SIZE = MAX_TEMP_SENDBUF_SIZE * 8;	// TCP���ջ�������Сֵ

// ������socket���
const unsigned int			CHECK_HEAETBEAT_SECS = 15;							// ������ʱ��ʱ��(s)
const unsigned int			KEEP_ACTIVE_HEARTBEAT_COUNT = 3;					// ǰ�˺ͷ�����������

#pragma pack(1)
//���ݶ�����Ϣͷ
struct DataLineHead
{
	unsigned int uSize;			//���ݴ�С
	unsigned int uDataKind;		//��������
	DataLineHead() { memset(this, 0, sizeof(DataLineHead)); }
};

//�������ݰ��ṹͷ
struct NetMessageHead
{
	unsigned int uMainID;							////����������
	unsigned int uAssistantID;						////������������ ID
	unsigned int uHandleCode;						////���ݰ��������
	unsigned int uIdentification;					////��ݱ�ʶ����ͬ��Э�������в�ͬ�ĺ��壩
	unsigned int uMessageSize;						////���ݰ���С

	NetMessageHead() { memset(this, 0, sizeof(NetMessageHead)); }
};

// �����߳���Ϣ
struct WorkThreadInfo
{
	struct event_base*	base;
	struct event*		event;   //read_fd�Ķ��¼�
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
	SOCKET			acceptFd;			//�Լ���socket
	bufferevent*	bev;
	std::mutex*		lock;
	bool			bHandleAccptMsg;	//�Ƿ�����������Ϣ��websocketʹ��

	TCPSocketInfo() { memset(this, 0, sizeof(TCPSocketInfo)); }
};

// Ͷ��soket
struct PlatformSocketInfo
{
	time_t			acceptMsgTime;
	char			ip[MAX_NUM_IP_ADDR_SIZE];
	unsigned short	port;
	SOCKET			acceptFd;			//�Լ���socket

	PlatformSocketInfo() { memset(this, 0, sizeof(PlatformSocketInfo)); }
};

// �������ݶ��а�ͷ
struct SendDataLineHead
{
	DataLineHead			dataLineHead;							//����ͷ
	int						socketIndex;							//socket���������ļ�������
	void*					pBufferevent;							//bufferevent		
	SendDataLineHead() { memset(this, 0, sizeof(SendDataLineHead)); }
};

// SOCKET��ȡ֪ͨ�ṹ����
struct SocketReadLine
{
	DataLineHead						LineHead;					//����ͷ
	NetMessageHead						netMessageHead;				//���ݰ�ͷ
	unsigned int						uHandleSize;				//���ݰ������С
	unsigned int						uIndex;						//SOCKET ����
	unsigned long int					uAccessIP;					//SOCKET IP
	void*								pBufferevent;				//bufferevent
	SocketType							socketType;					//socket���� enum SocketType

	SocketReadLine() { memset(this, 0, sizeof(SocketReadLine)); }
};

#pragma pack()
