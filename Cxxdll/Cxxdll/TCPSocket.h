#pragma once

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
	bool Start(ServiceType serverType);

public:
	bool SendData(int index, const char* pData, size_t size, MsgCmd mainID, int assistID, int handleCode, void* pBufferevent, unsigned int uIdentification = 0);
	void RemoveTCPSocketStatus(bool isClientAutoClose = false);

public:
	CDataLine* GetRecvDataLine();
	CDataLine* GetSendDataLine();
	ConditionVariable& GetConditionVariable();
	std::vector<std::thread*>& GetSockeThreadVec();
	TCPSocketInfo& GetTCPSocketInfo();

	bool& GetRuninged();
private:
	static void SetTcpRcvSndBUF(SOCKET fd, int rcvBufSize, int sndBufSize);
	static void SetMaxSingleReadAndWrite(bufferevent* bev, int rcvBufSize, int sndBufSize);

private:
	bool RecvData(bufferevent* bev);
	bool DispatchPacket(void* pBufferevent, NetMessageHead* pHead, void* pData, int size);
	bool OnSocketCloseEvent(ULONG uAccessIP, UINT uIndex, UINT uConnectTime, BYTE socketType);

private:
	// 线程入口
	void ConnectServerThread();
	void ThreadSendMsgThread();
	void HandleSendData(ListItemData* pListItem);

private:
	static void ReadCB(struct bufferevent*, void*);
	static void EventCB(struct bufferevent*, short, void*);

private:
	bool                        m_running;
	CDataLine*					m_pRecvDataLine;
	CDataLine*					m_pSendDataLine;
	event_config*				m_eventBaseCfg;
	ServiceType                 m_iServiceType;
	ConditionVariable           m_ConditionVariable;
	std::vector<std::thread*>   m_socketThread;
	TCPSocketInfo				m_socketInfo;
	event_base*					m_ConnectServerBase;
};