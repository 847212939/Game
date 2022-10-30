#include "../stdafx.h"

PlayerPrep::PlayerPrep() : m_pServerTimer(new CServerTimer[G_BaseCfgMgr.GetTimerCnt()])
{
}
PlayerPrep::~PlayerPrep()
{

}

// 初始化
void PlayerPrep::Init()
{
	int timerCnt = G_BaseCfgMgr.GetTimerCnt();
	for (int i = 0; i < timerCnt; i++)
	{
		m_pServerTimer[i].Start();
	}
	G_SceneClient->Init();
}

// 处理消息
void PlayerPrep::MessageDispatch(PlayerInfo* playerInfo)
{
	if (!playerInfo)
	{
		Log(CERR, "!playerInfo");
		return;
	}
	if (!playerInfo->pMsg)
	{
		Log(CERR, "!playerInfo->pMsg || !playerInfo->pTcpSockInfo");
		return;
	}
	auto* tcpInfo = G_NetClient->GetTCPSocketInfo(playerInfo->pMsg->uIndex);
	if (!tcpInfo)
	{
		Log(CERR, "!tcpInfo");
		return;
	}
	if (playerInfo->pMsg->uIndex != G_NetClient->GetDBServerIndex())
	{
		if (tcpInfo->link != (uint64_t)MsgCmd::MsgCmd_Testlink)
		{
			Log(CERR, "!tcpInfo->link != (uint64_t)MsgCmd::MsgCmd_Testlink");
			G_NetClient->CloseSocket(playerInfo->pMsg->uIndex);
			return;
		}
	}
	unsigned int uMainID = playerInfo->pMsg->netMessageHead.uMainID;
	if (uMainID >= (unsigned int)MsgCmd::MsgCmd_End || uMainID <= (unsigned int)MsgCmd::MsgCmd_Begin)
	{
		Log(CERR, "没有找到消息类型 cmd = %d", uMainID);
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
		Log(CERR, "playerInfo = null cmd = %d", (int)cmd);
		return;
	}
	SocketReadLine* pMsg = playerInfo->pMsg;
	if (!pMsg)
	{
		Log(CERR, "pMsg = null cmd = %d", (int)cmd);
		return;
	}
	// 处理登录协议等.. 玩家没有创建
	if (MsgCmd::MsgCmd_PlayerPreproces == (MsgCmd)pMsg->netMessageHead.uIdentification)
	{
		CallBackFun(cmd, playerInfo);
	}
	else
	{
		G_SceneClient->MessageDispatch(cmd, playerInfo);
	}
}

// 创建角色
void PlayerPrep::CreatePlayer(LoginData& loginData)
{
	G_PlayerCenterClient->CreatePlayer(loginData);
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

	Log(CERR, "There is already a callback for this message. Please check the code cmd = %d", cmd);
}

// 定时器
bool PlayerPrep::SetTimer(TimerCmd uTimerID, unsigned int uElapse, unsigned char timerType/* = SERVERTIMER_TYPE_PERISIST*/)
{
	if (!m_pServerTimer)
	{
		Log(CERR, "no timer run");
		return false;
	}

	int timerCnt = G_BaseCfgMgr.GetTimerCnt();

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

	int timerCnt = G_BaseCfgMgr.GetTimerCnt();

	if (timerCnt <= 0 || timerCnt > MAX_TIMER_THRED_NUMS)
	{
		Log(CERR, "timer error");
		return false;
	}

	m_pServerTimer[(int)uTimerID % timerCnt].KillTimer((unsigned int)uTimerID);

	return true;
}