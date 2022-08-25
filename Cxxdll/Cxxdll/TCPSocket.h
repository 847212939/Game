#pragma once
#include "Structural.h"
#include "ConditionVariable.h"

class CTCPSocketManage
{
public:
	CTCPSocketManage();
	virtual ~CTCPSocketManage();

protected:
	CTCPSocketManage(const CTCPSocketManage& my);
	CTCPSocketManage& operator=(const CTCPSocketManage& my);

public:
	bool Stop();
	bool Start();
	bool InitNetwork(char* ip, int port, int timerCnt);
	int GetTimerCnt();

public:
	bool SendData(const char* pData, size_t size, int mainID, int assistID, int handleCode, void* pBufferevent, unsigned int uIdentification = 0);
	void RemoveTCPSocketStatus(bool isClientAutoClose = false);

public:
	bool& GetRuninged();
	CDataLine* GetRecvDataLine();
	CDataLine* GetSendDataLine();
	ConditionVariable& GetConditionVariable();
	std::vector<std::thread*>& GetSockeThreadVec();
	TCPSocketInfo& GetTCPSocketInfo();

private:
	bool ConnectServer(SockFd& fd);

private:
	static void SetTcpRcvSndBUF(SockFd fd, int rcvBufSize, int sndBufSize);
	static void SetMaxSingleReadAndWrite(bufferevent* bev, int rcvBufSize, int sndBufSize);

private:
	bool RecvData(bufferevent* bev);
	bool DispatchPacket(void* pBufferevent, NetMessageHead* pHead, void* pData, int size);
	bool OnSocketCloseEvent(unsigned long uAccessIP, unsigned int uIndex, unsigned int uConnectTime, unsigned char socketType);

private:
	void ConnectServerThread(SockFd& fd);
	void ThreadSendMsgThread();
	void HandleSendData(ListItemData* pListItem);

private:
	static void ReadCB(struct bufferevent*, void*);
	static void EventCB(struct bufferevent*, short, void*);

private:
	int							m_timerCnt;
	int							m_port;
	char						m_ip[64];
	bool                        m_running;
	CDataLine*					m_pRecvDataLine;
	CDataLine*					m_pSendDataLine;
	event_config*				m_eventBaseCfg;
	ServiceType                 m_iServiceType;
	ConditionVariable           m_ConditionVariable;
	std::vector<std::thread*>   m_socketThread;
	TCPSocketInfo				m_socketInfo;
	event_base*					m_ConnectServerBase;
	SockFd						m_socket;
};