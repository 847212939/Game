#pragma once

class CServerTimer
{
	typedef std::unordered_map<unsigned int, ServerTimerInfo> ServerTimerInfomap;
public:
	CServerTimer();
	~CServerTimer();

public:
	bool Start(int timeonce = 100);
	bool Stop();
	bool SetTimer(unsigned int uTimerID, unsigned int uElapse, BYTE timerType = SERVERTIMER_TYPE_PERISIST); //uElapse是毫秒单位，大于100ms
	bool KillTimer(unsigned int uTimerID);
	bool ExistsTimer(unsigned int uTimerID);

public:
	void SetTimerRun(bool run);
	void SetTCPClient(TCPClient* pTCPClient);

private:
	void ThreadCheckTimer();
	static void TimeoutCB(evutil_socket_t fd, short event, void* arg);

private:
	int					m_timeOnce;
	volatile bool		m_bRun;
	TCPClient*			m_TCPClient;
	ConditionVariable	m_cond;	
	ServerTimerInfomap  m_timerMap;
};