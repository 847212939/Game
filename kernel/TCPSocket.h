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
		// ��ʼ��
		virtual bool Init(int maxCount, int port, const char* ip = NULL, SocketType socketType = SocketType::SOCKET_TYPE_TCP);
		// ȡ����ʼ��
		virtual bool UnInit();
		// ��ʼ����
		virtual bool Start(SocketType serverType);
		// ֹͣ����
		virtual bool Stop();

	public:
		// �������ݺ���
		virtual bool SendData(int index, void* pData, int size, int mainID, int assistID, int handleCode, void* pBufferevent, unsigned int uIdentification = 0);
		// �ر�����(ҵ���߼��̵߳���)
		bool CloseSocket(int index);
		// ��ȡ����dataline
		CDataLine* GetRecvDataLine();
		// ��ȡ����dataline
		CDataLine* GetSendDataLine();
		// ��ȡ��ǰsocket��������
		unsigned int GetCurSocketSize();
		// �ж�socket�Ƿ�����
		bool IsConnected(int index);
		// ��ȡsocketSet
		void GetSocketSet(std::vector<unsigned int>& vec);
		// ��ȡsocketVector
		const std::vector<TCPSocketInfo>& GetSocketVector();
		// ��ȡ����ip
		const char* GetSocketIP(int index);
		// ��ȡTcpSocketInfo
		const TCPSocketInfo* GetTCPSocketInfo(int index);
		// ��ȡ�߳̾������
		std::vector<std::thread*>& GetSockeThreadVec();
		// ��ȡ�߳�����״̬
		bool GetRuninged();
		// ��ȡ����������
		SocketType GetServerType();

	private:
		// ����tcp����
		// ����tcp�շ�������
		static void SetTcpRcvSndBUF(SOCKET fd, int rcvBufSize, int sndBufSize);
		// ����Ӧ�ò㵥�ζ�ȡ���ݰ��Ĵ�С bufferevent_set_max_single_read
		static void SetMaxSingleReadAndWrite(bufferevent* bev, int rcvBufSize, int sndBufSize);

		// ����socketIndex�㷨
		int GetSocketIndex();
		// ���TCPSocketInfo
		void AddTCPSocketInfo(int threadIndex, PlatformSocketInfo* pTCPSocketInfo);
		// ����tcpΪδ����״̬
		void RemoveTCPSocketStatus(int index, bool isClientAutoClose = false);
		// �ɷ����ݰ�
		bool DispatchPacket(void* pBufferevent, int index, NetMessageHead* pHead, void* pData, int size);
		// ��ײ㴦���յ������ݺ���
		bool RecvData(bufferevent* bev, int index);

		// SOCKET ����Ӧ���߳�
		static void ThreadAccept(void* pThreadData);
		// SOCKET ���ݽ����߳�
		static void ThreadRSSocket(void* pThreadData);
		// SOCKET ���ݷ����߳�
		static void ThreadSendMsg(void* pThreadData);

		// �ص�����
		// �µ����ӵ�����ThreadAccept�̺߳���
		static void ListenerCB(struct evconnlistener*, evutil_socket_t, struct sockaddr*, int socklen, void*);
		// �µ����ݵ�����ThreadRSSocket�̺߳���
		static void ReadCB(struct bufferevent*, void*);
		// ���ӹرյȵȴ�����Ϣ��ThreadRSSocket�̺߳���
		static void EventCB(struct bufferevent*, short, void*);
		// acceptʧ�ܣ�ThreadAccept�̺߳���
		static void AcceptErrorCB(struct evconnlistener* listener, void*);
		// �µ����ӵ�����ThreadRSSocket�̺߳���
		static void ThreadLibeventProcess(evutil_socket_t readfd, short which, void* arg);

		//�Զ���socketPair
		static int StreamSocketpair(struct addrinfo* addr_info, SOCKET sock[2]);
		static int DgramSocketpair(struct addrinfo* addr_info, SOCKET sock[2]);
		static int Socketpair(int family, int type, int protocol, SOCKET recv[2]);
	private:
		event_base*					m_listenerBase;
		std::vector<WorkThreadInfo> m_workBaseVec;
		ConditionVariable			m_ConditionVariable;

		CDataLine*					m_pSendDataLine;	//���Ͷ���
		CDataLine*					m_pRecvDataLine;	//���ܶ���

		std::vector<TCPSocketInfo>	m_socketInfoVec;
		std::set<unsigned int>		m_heartBeatSocketSet;

		bool						m_running;
		char						m_bindIP[48];
		unsigned short				m_port;
		SocketType					m_socketType;
		unsigned int				m_uMaxSocketSize; // libevent ���߳�Ĭ�ϵ�32000
		unsigned int				m_uCurSocketSize;
		unsigned int				m_uCurSocketIndex;

		SocketType					m_iServiceType;
		std::vector<std::thread*>	m_socketThread;
	};
}
