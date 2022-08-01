#include "../Game/stdafx.h"

struct TimerParam
{
	CServerTimer* pCServerTimer;
	struct event_base* base;
};

CServerTimer::CServerTimer() : m_bRun(false), m_timeOnce(100)
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
		COUT_LOG(LOG_CERROR, "timeonce=%d 不满足要求", timeonce);
		return false;
	}

	m_bRun = true;
	m_timeOnce = timeonce;

	DTCPClient.GetSockeThreadVec().push_back(new std::thread(&CServerTimer::ThreadCheckTimer, this));

	return true;
}

void CServerTimer::ThreadCheckTimer()
{
	struct event timeout;
	struct event_base* base;

	/* Initialize the event library */
	base = event_base_new();

	TimerParam param;
	param.base = base;
	param.pCServerTimer = this;

	/* Initialize one event */
	event_assign(&timeout, base, -1, EV_PERSIST, CServerTimer::TimeoutCB, (void*)&param);

	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = m_timeOnce * 1000;
	event_add(&timeout, &tv);

	event_base_dispatch(base);

	event_base_free(base);
}

void CServerTimer::TimeoutCB(evutil_socket_t fd, short event, void* arg)
{
	struct TimerParam* param = (struct TimerParam*)arg;
	if (param == nullptr)
	{
		COUT_LOG(LOG_CERROR, "TimeoutCB 参数为空");
		return;
	}
	CServerTimer* pCServerTimer = param->pCServerTimer;
	if (!pCServerTimer)
	{
		COUT_LOG(LOG_CERROR, "pCServerTimer 参数为空");
		return;
	}
	struct event_base* base = param->base;
	if (!base)
	{
		COUT_LOG(LOG_CERROR, "base 参数为空");
		return;
	}
	CDataLine* pCDataLine = DTCPClient.GetRecvDataLine();
	if (!pCDataLine)
	{
		COUT_LOG(LOG_CERROR, "pDataLine == nullptr");
		return;
	}
	if (!pCServerTimer->m_bRun)
	{
		event_base_loopbreak(base);
	}

	long long currTime = Util::GetSysMilliseconds() / pCServerTimer->m_timeOnce * pCServerTimer->m_timeOnce;

	ServerTimerInfomap& timerMap = pCServerTimer->m_timerMap;
	// lock
	pCServerTimer->m_cond.GetMutex().lock();

	for (ServerTimerInfomap::iterator iter = timerMap.begin(); iter != timerMap.end();)
	{
		if ((currTime >= iter->second.starttime) && (currTime - iter->second.starttime) % iter->second.elapse == 0)
		{
			ServerTimerLine WindowTimer;
			WindowTimer.uMainID = (unsigned int)MsgCmd::MsgCmd_Timer;
			WindowTimer.uTimerID = iter->first;
			pCDataLine->AddData(&WindowTimer, sizeof(ServerTimerLine), HD_TIMER_MESSAGE);

			if (iter->second.timertype == SERVERTIMER_TYPE_SINGLE)
			{
				timerMap.erase(iter++);
				continue;
			}
		}

		iter++;
	}

	pCServerTimer->m_cond.GetMutex().unlock();
}

bool CServerTimer::Stop()
{
	COUT_LOG(LOG_CINFO, "CServerTimer thread exit.");

	m_bRun = false;
	m_timerMap.clear();

	return true;
}

bool CServerTimer::SetTimer(unsigned int uTimerID, unsigned int uElapse, BYTE timerType /*= SERVERTIMER_TYPE_PERISIST*/)
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
