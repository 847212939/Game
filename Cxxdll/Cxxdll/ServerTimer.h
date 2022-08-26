#pragma once
#include "Structural.h"
#include "ConditionVariable.h"

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
	bool SetTimer(unsigned int uTimerID, unsigned int uElapse, unsigned char timerType = SERVERTIMER_TYPE_PERISIST); //uElapse是毫秒单位，大于100ms
	bool KillTimer(unsigned int uTimerID);
	bool ExistsTimer(unsigned int uTimerID);

public:
	void SetTimerRun(bool run);
	struct event_base* GetBase();

private:
	void ThreadCheckTimer();
	static void TimeoutCB(evutil_socket_t fd, short event, void* arg);

private:
	int					m_timeOnce;
	volatile bool		m_bRun;
	ConditionVariable	m_cond;
	ServerTimerInfomap  m_timerMap;
	struct event_base*  m_base;
};