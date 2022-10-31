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
	bool SendMsg(const char* pData, size_t size, int mainID, int assistID, int handleCode, unsigned int uIdentification = 0);
	void RemoveTCPSocketStatus(bool isClientAutoClose = false);

public:
	const SockFd& GetSocket() const;
	bool& GetRuninged();
	CDataLine* GetRecvDataLine();
	CDataLine* GetSendDataLine();
	struct bufferevent* GetScoketbev();

private:
	bool ConnectServer();

private:
	static void SetTcpRcvSndBUF(SockFd fd, int rcvBufSize, int sndBufSize);
	static void SetMaxSingleReadAndWrite(bufferevent* bev, int rcvBufSize, int sndBufSize);

private:
	bool RecvData(bufferevent* bev);
	bool DispatchPacket(void* pBufferevent, NetMessageHead* pHead, void* pData, int size);
	bool OnSocketCloseEvent(unsigned int uIndex);

private:
	void ThreadSendMsgThread();
	void HandleSendData(ListItemData* pListItem);

private:
	static void ReadCB(struct bufferevent*, void*);
	static void EventCB(struct bufferevent*, short, void*);

private:
	bool VerifyConnection(char* data);

private:
	int							m_timerCnt;
	int							m_port;
	std::string					m_ip;
	bool                        m_running;
	bool						m_Connected;
	CDataLine*					m_pRecvDataLine;
	CDataLine*					m_pSendDataLine;
	event_config*				m_eventBaseCfg;
	ServiceType                 m_ServiceType;
	struct bufferevent*			m_Socketbev;
	SockFd						m_socket;
};