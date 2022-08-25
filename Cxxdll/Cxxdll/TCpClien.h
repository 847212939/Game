#pragma once
#include "Structural.h"
#include "ConditionVariable.h"

class TCPClient : public CTCPSocketManage
{
public:
	TCPClient();
	virtual ~TCPClient();

protected:
	TCPClient(const TCPClient& my);
	TCPClient& operator=(const TCPClient& my);

public:
	bool Init(NetworkCallBackFunc netFunc, TimerCallBackFunc timerFunc);
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
	void AddTimerCallback(int cmd);
	bool CallBackFun(int cmd);
	void DelTimerCallback(int cmd);
	bool SetTimer(int uTimerID, unsigned int uElapse, unsigned char timerType = SERVERTIMER_TYPE_PERISIST);
	bool KillTimer(int uTimerID);

private:
	TypeFunMap			m_TypeFunMap;
	CServerTimer*		m_pServerTimer;
	TimerFunMap			m_TimerFunMap;
	NetworkCallBackFunc	m_NetworkCallBackFunc;
	TimerCallBackFunc	m_TimerCallBackFunc;
};