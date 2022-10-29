#include "pch.h"

PlayerPrep::PlayerPrep() : m_pServerTimer(new CServerTimer[G_CfgMgr->GetCBaseCfgMgr().GetTimerCnt()])
{
}
PlayerPrep::~PlayerPrep()
{

}

// 初始化
void PlayerPrep::Init()
{
	int timerCnt = G_CfgMgr->GetCBaseCfgMgr().GetTimerCnt();
	for (int i = 0; i < timerCnt; i++)
	{
		m_pServerTimer[i].Start();
	}
}

CServerTimer* PlayerPrep::GetCServerTimer()
{
	return m_pServerTimer;
}

void PlayerPrep::AddNetCallback(MsgCmd cmd, std::function<void(PlayerInfo*)>&& fun)
{
	MapNetFun::iterator it = m_NetCBFunMap.find(cmd);
	if (it == m_NetCBFunMap.end())
	{
		m_NetCBFunMap.insert(std::make_pair(cmd, fun));
		return;
	}

	Log(CERR, "There is already a callback for this message. Please check the code cmd = %d", cmd);
}
bool PlayerPrep::CallBackFun(MsgCmd cmd, PlayerInfo* playerInfo)
{
	MapNetFun::iterator it = m_NetCBFunMap.find(cmd);
	if (it == m_NetCBFunMap.end())
	{
		Log(CERR, "No corresponding callback function found cmd = %d", cmd);
		return false;
	}

	it->second(playerInfo);
	return true;
}
bool PlayerPrep::CallBackFun(TimerCmd cmd)
{
	MapTimerFunc::iterator it = m_TimerFunMap.find(cmd);
	if (it == m_TimerFunMap.end())
	{
		Log(CERR, "No corresponding callback function found cmd = %d", cmd);
		return false;
	}

	it->second((int)cmd);
	return true;
}
void PlayerPrep::DelTimerCallback(TimerCmd cmd)
{
	MapTimerFunc::iterator it = m_TimerFunMap.find(cmd);
	if (it == m_TimerFunMap.end())
	{
		return;
	}

	m_TimerFunMap.erase(it);
}
void PlayerPrep::AddTimerCallback(TimerCmd cmd)
{
	MapTimerFunc::iterator it = m_TimerFunMap.find(cmd);
	if (it == m_TimerFunMap.end())
	{
		m_TimerFunMap.insert(std::make_pair(cmd, G_NetClient->GetTimerCallBackFunc()));
		return;
	}

	Log(CERR, "There is already a callback for this message. Please check the code cmd = %d", cmd);
}

// 定时器
bool PlayerPrep::SetTimer(TimerCmd uTimerID, unsigned int uElapse,
	unsigned char timerType/* = SERVERTIMER_TYPE_PERISIST*/)
{
	if (!m_pServerTimer)
	{
		Log(CERR, "no timer run");
		return false;
	}

	int timerCnt = G_CfgMgr->GetCBaseCfgMgr().GetTimerCnt();

	if (timerCnt <= 0 || timerCnt > MAX_TIMER_THRED_NUMS)
	{
		Log(CERR, "timer error");
		return false;
	}

	m_pServerTimer[(int)uTimerID % timerCnt].SetTimer((unsigned int)uTimerID, uElapse, timerType);

	return true;
}
bool PlayerPrep::KillTimer(TimerCmd uTimerID)
{
	if (!m_pServerTimer)
	{
		Log(CERR, "no timer run");
		return false;
	}

	int timerCnt = G_CfgMgr->GetCBaseCfgMgr().GetTimerCnt();

	if (timerCnt <= 0 || timerCnt > MAX_TIMER_THRED_NUMS)
	{
		Log(CERR, "timer error");
		return false;
	}

	m_pServerTimer[(int)uTimerID % timerCnt].KillTimer((unsigned int)uTimerID);

	return true;
}