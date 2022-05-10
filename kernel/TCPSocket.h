#pragma once
#include "ConditionVariable.h"
#include "Internal.h"
#include <vector>
#include <set>
#include "Util.h"
#include "DataLine.h"

namespace DRMU
{
	class CTCPSocketManage
	{
	public:
		CTCPSocketManage();
		virtual ~CTCPSocketManage();

	public:
		// 初始化
		virtual bool Init(int maxCount, int port, const char* ip = NULL, SocketType socketType = SocketType::SOCKET_TYPE_TCP);
		// 取消初始化
		virtual bool UnInit();
		// 开始服务
		virtual bool Start(SocketType serverType);
		// 停止服务
		virtual bool Stop();

	public:
		// 发送数据函数
		virtual bool SendData(int index, void* pData, int size, int mainID, int assistID, int handleCode, void* pBufferevent, unsigned int uIdentification = 0);
		// 关闭连接(业务逻辑线程调用)
		bool CloseSocket(int index);
		// 获取接收dataline
		CDataLine* GetRecvDataLine();
		// 获取发送dataline
		CDataLine* GetSendDataLine();
		// 获取当前socket连接总数
		unsigned int GetCurSocketSize();
		// 判断socket是否连接
		bool IsConnected(int index);
		// 获取socketSet
		void GetSocketSet(std::vector<unsigned int>& vec);
		// 获取socketVector
		const std::vector<TCPSocketInfo>& GetSocketVector();
		// 获取连接ip
		const char* GetSocketIP(int index);
		// 获取TcpSocketInfo
		const TCPSocketInfo* GetTCPSocketInfo(int index);
		// 获取线程句柄队列
		std::vector<std::thread*>& GetSockeThreadVec();
		// 获取线程运行状态
		bool GetRuninged();
		// 获取服务器类型
		SocketType GetServerType();

	private:
		// 设置tcp属性
		// 设置tcp收发缓冲区
		static void SetTcpRcvSndBUF(SOCKET fd, int rcvBufSize, int sndBufSize);
		// 设置应用层单次读取数据包的大小 bufferevent_set_max_single_read
		static void SetMaxSingleReadAndWrite(bufferevent* bev, int rcvBufSize, int sndBufSize);

		// 分配socketIndex算法
		int GetSocketIndex();
		// 添加TCPSocketInfo
		void AddTCPSocketInfo(int threadIndex, PlatformSocketInfo* pTCPSocketInfo);
		// 设置tcp为未连接状态
		void RemoveTCPSocketStatus(int index, bool isClientAutoClose = false);
		// 派发数据包
		bool DispatchPacket(void* pBufferevent, int index, NetMessageHead* pHead, void* pData, int size);
		// 最底层处理收到的数据函数
		bool RecvData(bufferevent* bev, int index);

		// SOCKET 连接应答线程
		static void ThreadAccept(void* pThreadData);
		// SOCKET 数据接收线程
		static void ThreadRSSocket(void* pThreadData);
		// SOCKET 数据发送线程
		static void ThreadSendMsg(void* pThreadData);

		// 回调函数
		// 新的连接到来，ThreadAccept线程函数
		static void ListenerCB(struct evconnlistener*, evutil_socket_t, struct sockaddr*, int socklen, void*);
		// 新的数据到来，ThreadRSSocket线程函数
		static void ReadCB(struct bufferevent*, void*);
		// 连接关闭等等错误消息，ThreadRSSocket线程函数
		static void EventCB(struct bufferevent*, short, void*);
		// accept失败，ThreadAccept线程函数
		static void AcceptErrorCB(struct evconnlistener* listener, void*);
		// 新的连接到来，ThreadRSSocket线程函数
		static void ThreadLibeventProcess(evutil_socket_t readfd, short which, void* arg);

		//自定义socketPair
		static int StreamSocketpair(struct addrinfo* addr_info, SOCKET sock[2]);
		static int DgramSocketpair(struct addrinfo* addr_info, SOCKET sock[2]);
		static int Socketpair(int family, int type, int protocol, SOCKET recv[2]);
	private:
		event_base*					m_listenerBase;
		std::vector<WorkThreadInfo> m_workBaseVec;
		ConditionVariable			m_ConditionVariable;

		CDataLine*					m_pSendDataLine;	//发送队列
		CDataLine*					m_pRecvDataLine;	//接受对列

		std::vector<TCPSocketInfo>	m_socketInfoVec;
		std::set<unsigned int>		m_heartBeatSocketSet;

		bool						m_running;
		char						m_bindIP[48];
		unsigned short				m_port;
		SocketType					m_socketType;
		unsigned int				m_uMaxSocketSize; // libevent 单线程默认的32000
		unsigned int				m_uCurSocketSize;
		unsigned int				m_uCurSocketIndex;

		SocketType					m_iServiceType;
		std::vector<std::thread*>	m_socketThread;
	};
}
