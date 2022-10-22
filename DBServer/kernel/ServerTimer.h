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
	// uElapse�Ǻ��뵥λ������100ms
	bool SetTimer(unsigned int uTimerID, unsigned int uElapse, unsigned char timerType = SERVERTIMER_TYPE_PERISIST);
	// ֹͣ��ʱ��
	bool KillTimer(unsigned int uTimerID);
	// ��ʱ���Ƿ���
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
	MapServerTimerInfo  m_timerMap;
};