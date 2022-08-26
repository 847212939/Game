#include "pch.h"

CServerTimer::CServerTimer() : m_bRun(false), m_timeOnce(100), m_base(nullptr)
{
}

CServerTimer::~CServerTimer()
{
	m_timerMap.clear();
}

bool CServerTimer::Start(int timeonce/* = 100*/)
{
	if (timeonce != 100 && timeonce != 1000)
	{
		return false;
	}

	m_bRun = true;
	m_timeOnce = timeonce;

	std::thread threadCheckTimer(&CServerTimer::ThreadCheckTimer, this);
	threadCheckTimer.detach();

	return true;
}

void CServerTimer::ThreadCheckTimer()
{
	struct event timeout;
	if (!m_base)
	{
		m_base = event_base_new();
	}

	event_assign(&timeout, m_base, -1, EV_PERSIST, CServerTimer::TimeoutCB, (void*)this);

	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = m_timeOnce * 1000;
	event_add(&timeout, &tv);

	event_base_dispatch(m_base);
	event_base_free(m_base);
	m_base = nullptr;
}

void CServerTimer::TimeoutCB(evutil_socket_t fd, short event)
{
	if (!m_base)
	{
		return;
	}
	CDataLine* pCDataLine = DUtil->GetTCPClient()->GetRecvDataLine();
	if (!pCDataLine)
	{
		return;
	}
	if (!m_bRun)
	{
		event_base_loopbreak(m_base);
	}

	long long currTime = Util::GetSysMilliseconds() / m_timeOnce * m_timeOnce;

	// lock
	{
		m_cond.GetMutex().lock();
		for (ServerTimerInfomap::iterator iter = m_timerMap.begin(); iter != m_timerMap.end();)
		{
			if ((currTime >= iter->second.starttime) && 
				(currTime - iter->second.starttime) % iter->second.elapse == 0)
			{
				ServerTimerLine WindowTimer;
				WindowTimer.uMainID = 6;
				WindowTimer.uTimerID = iter->first;
				pCDataLine->AddData(&WindowTimer, sizeof(ServerTimerLine), SysMsgCmd::HD_TIMER_MESSAGE);

				if (iter->second.timertype == SERVERTIMER_TYPE_SINGLE)
				{
					m_timerMap.erase(iter++);
					continue;
				}
			}

			iter++;
		}
		m_cond.GetMutex().unlock();
	}
}

void CServerTimer::TimeoutCB(evutil_socket_t fd, short event, void* arg)
{
	CServerTimer* pCServerTimer = (CServerTimer*)arg;
	if (!pCServerTimer)
	{
		return;
	}
	pCServerTimer->TimeoutCB(fd, event);
}

bool CServerTimer::SetTimer(unsigned int uTimerID, unsigned int uElapse, unsigned char timerType /*= SERVERTIMER_TYPE_PERISIST*/)
{
	if (uElapse < (unsigned int)m_timeOnce)
	{
		uElapse = m_timeOnce;
	}

	ServerTimerInfo info;

	info.elapse = uElapse / m_timeOnce * m_timeOnce;
	info.starttime = Util::GetSysMilliseconds() / m_timeOnce * m_timeOnce + uElapse;
	info.timertype = timerType;

	std::lock_guard<std::mutex> guard(m_cond.GetMutex());

	m_timerMap[uTimerID] = info;

	return true;
}

bool CServerTimer::KillTimer(unsigned int uTimerID)
{
	if (!ExistsTimer(uTimerID))
	{
		return false;
	}

	std::lock_guard<std::mutex> guard(m_cond.GetMutex());

	auto iter = m_timerMap.find(uTimerID);
	if (iter == m_timerMap.end())
	{
		return false;
	}

	m_timerMap.erase(iter);

	return true;
}

bool CServerTimer::ExistsTimer(unsigned int uTimerID)
{
	auto iter = m_timerMap.find(uTimerID);
	if (iter == m_timerMap.end())
	{
		return false;
	}

	return true;
}

void CServerTimer::SetTimerRun(bool run)
{
	m_bRun = run;
}

struct event_base* CServerTimer::GetBase()
{
	return m_base;
}
