#pragma once

class CServerTimer
{
public:
	CServerTimer();
	~CServerTimer();

protected:
	CServerTimer(const CServerTimer& my);
	CServerTimer& operator=(const CServerTimer& my);

public:
	bool Start(int timeonce = 100);
	bool Stop();
	bool SetTimer(unsigned int uTimerID, unsigned int uElapse, BYTE timerType = SERVERTIMER_TYPE_PERISIST); //uElapse是毫秒单位，大于100ms
	bool KillTimer(unsigned int uTimerID);
	bool ExistsTimer(unsigned int uTimerID);

public:
	void SetTimerRun(bool run);

private:
	void ThreadCheckTimer();
	static void TimeoutCB(evutil_socket_t fd, short event, void* arg);

private:
	int					m_timeOnce;
	volatile bool		m_bRun;
	ConditionVariable	m_cond;
	ServerTimerInfomap  m_timerMap;
};