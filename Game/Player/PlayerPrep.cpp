#include "../stdafx.h"

PlayerPrep::PlayerPrep() : m_pServerTimer(new CServerTimer[BaseCfgMgr.GetTimerCnt()])
{
}
PlayerPrep::~PlayerPrep()
{

}

// 初始化
void PlayerPrep::Init()
{
	int timerCnt = BaseCfgMgr.GetTimerCnt();
	for (int i = 0; i < timerCnt; i++)
	{
		m_pServerTimer[i].Start();
	}
	DSC->Init();
}

// 处理消息
void PlayerPrep::MessageDispatch(PlayerInfo* playerInfo)
{
	if (!playerInfo)
	{
		COUT_LOG(LOG_CERROR, "!playerInfo");
		return;
	}
	if (!playerInfo->pMsg)
	{
		COUT_LOG(LOG_CERROR, "!playerInfo->pMsg || !playerInfo->pTcpSockInfo");
		return;
	}
	auto* tcpInfo = DTCPC->GetTCPSocketInfo(playerInfo->pMsg->uIndex);
	if (!tcpInfo)
	{
		COUT_LOG(LOG_CERROR, "!tcpInfo");
		return;
	}
	if (playerInfo->pMsg->uIndex != DTCPC->GetDBServerIndex())
	{
		if (tcpInfo->link != (uint64_t)MsgCmd::MsgCmd_Testlink)
		{
			COUT_LOG(LOG_CERROR, "!tcpInfo->link != (uint64_t)MsgCmd::MsgCmd_Testlink");
			DTCPC->CloseSocket(playerInfo->pMsg->uIndex);
			return;
		}
	}
	unsigned int uMainID = playerInfo->pMsg->netMessageHead.uMainID;
	if (uMainID >= (unsigned int)MsgCmd::MsgCmd_End || uMainID <= (unsigned int)MsgCmd::MsgCmd_Begin)
	{
		COUT_LOG(LOG_CERROR, "没有找到消息类型 cmd = %d", uMainID);
		return;
	}
	// websocket服务器
	if (playerInfo->pMsg->socketType == SocketType::SOCKET_TYPE_WEBSOCKET)
	{
		MessageDispatch((MsgCmd)uMainID, playerInfo);
	}
	else if (playerInfo->pMsg->socketType == SocketType::SOCKET_TYPE_TCP)
	{
		MessageDispatch((MsgCmd)uMainID, playerInfo);
	}
}
void PlayerPrep::MessageDispatch(MsgCmd cmd, PlayerInfo* playerInfo)
{
	if (!playerInfo)
	{
		COUT_LOG(LOG_CERROR, "playerInfo = null cmd = %d", (int)cmd);
		return;
	}
	SocketReadLine* pMsg = playerInfo->pMsg;
	if (!pMsg)
	{
		COUT_LOG(LOG_CERROR, "pMsg = null cmd = %d", (int)cmd);
		return;
	}
	// 处理登录协议等.. 玩家没有创建
	if (MsgCmd::MsgCmd_PlayerPreproces == (MsgCmd)pMsg->netMessageHead.uIdentification)
	{
		CallBackFun(cmd, playerInfo);
	}
	else
	{
		DSC->MessageDispatch(cmd, playerInfo);
	}
}

// 创建角色
void PlayerPrep::CreatePlayer(LoginData& loginData)
{
	DPCC->CreatePlayer(loginData);
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

	COUT_LOG(LOG_CERROR, "There is already a callback for this message. Please check the code cmd = %d", cmd);
}
bool PlayerPrep::CallBackFun(MsgCmd cmd, PlayerInfo* playerInfo)
{
	MapNetFun::iterator it = m_NetCBFunMap.find(cmd);
	if (it == m_NetCBFunMap.end())
	{
		COUT_LOG(LOG_CERROR, "No corresponding callback function found cmd = %d", cmd);
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
		COUT_LOG(LOG_CERROR, "No corresponding callback function found cmd = %d", cmd);
		return false;
	}

	it->second();
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
void PlayerPrep::AddTimerCallback(TimerCmd cmd, std::function<void()>&& fun)
{
	MapTimerFunc::iterator it = m_TimerFunMap.find(cmd);
	if (it == m_TimerFunMap.end())
	{
		m_TimerFunMap.insert(std::make_pair(cmd, fun));
		return;
	}

	COUT_LOG(LOG_CERROR, "There is already a callback for this message. Please check the code cmd = %d", cmd);
}

// 定时器
bool PlayerPrep::SetTimer(TimerCmd uTimerID, unsigned int uElapse, unsigned char timerType/* = SERVERTIMER_TYPE_PERISIST*/)
{
	if (!m_pServerTimer)
	{
		COUT_LOG(LOG_CERROR, "no timer run");
		return false;
	}

	int timerCnt = BaseCfgMgr.GetTimerCnt();

	if (timerCnt <= 0 || timerCnt > MAX_TIMER_THRED_NUMS)
	{
		COUT_LOG(LOG_CERROR, "timer error");
		return false;
	}

	m_pServerTimer[(int)uTimerID % timerCnt].SetTimer((unsigned int)uTimerID, uElapse, timerType);

	return true;
}
bool PlayerPrep::KillTimer(TimerCmd uTimerID)
{
	if (!m_pServerTimer)
	{
		COUT_LOG(LOG_CERROR, "no timer run");
		return false;
	}

	int timerCnt = BaseCfgMgr.GetTimerCnt();

	if (timerCnt <= 0 || timerCnt > MAX_TIMER_THRED_NUMS)
	{
		COUT_LOG(LOG_CERROR, "timer error");
		return false;
	}

	m_pServerTimer[(int)uTimerID % timerCnt].KillTimer((unsigned int)uTimerID);

	return true;
}