#pragma once

class TCPClient : public CTCPSocketManage
{
public:
	TCPClient();
	virtual ~TCPClient();

protected:
	TCPClient(const TCPClient& my);
	TCPClient& operator=(const TCPClient& my);

public:
	bool Init(pfCallBackEvent func);
	void NotifyAll();

private:
	void TimerCallback(void* pDataLineHead);
	void SocketCallback(void* pDataLineHead);
	bool CallBackFun(SysMsgCmd cmd, void* pDataLineHead);
	void AddNetTypeCallback(SysMsgCmd cmd, std::function<void(void* pDataLineHead)>&& fun);

private:
	void HandlerRecvDataListThread();
	void HandleRecvData(ListItemData* pListItem);

public:
	void AddTimerCallback(int cmd, std::function<void()>&& fun);
	bool CallBackFun(int cmd);
	void DelTimerCallback(int cmd);
	bool SetTimer(int uTimerID, UINT uElapse, BYTE timerType = SERVERTIMER_TYPE_PERISIST);
	bool KillTimer(int uTimerID);

private:
	TypeFunMap			m_TypeFunMap;
	pfCallBackEvent		m_CallBackFunc;
	CServerTimer*		m_pServerTimer;
	TimerFunMap			m_TimerFunMap;
};