#include "../stdafx.h"

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
	G_SceneClient->Init();
}

void PlayerPrep::MessageLogicDispatch(PlayerInfo* playerInfo)
{
	if (!playerInfo)
	{
		Log(CERR, "!playerInfo");
		return;
	}
	SocketReadLine* pMsg = playerInfo->pMsg;
	if (!pMsg)
	{
		Log(CERR, "!pMsg");
		return;
	}
	int index = pMsg->uIndex;
	TCPSocketInfo* tcpInfo = G_NetClient->GetTCPSocketInfo(index);
	if (!tcpInfo)
	{
		Log(CERR, "!tcpInfo");
		return;
	}
	if (!G_NetClient->IsServerMsg(index))
	{
		if (tcpInfo->link != (uint64_t)MsgCmd::MsgCmd_Testlink)
		{
			Log(CERR, "!tcpInfo->link != (uint64_t)MsgCmd::MsgCmd_Testlink");
			G_NetClient->CloseSocket(index);
			return;
		}
	}
	MsgCmd cmd = (MsgCmd)pMsg->netMessageHead.uMainID;
	if (cmd >= MsgCmd::MsgCmd_End || cmd <= MsgCmd::MsgCmd_Begin)
	{
		Log(CERR, "非法消息cmd=%d", (int)cmd);
		return;
	}
	MsgCmd identification = (MsgCmd)pMsg->netMessageHead.uIdentification;
	if (MsgCmd::MsgCmd_PlayerPreproces == identification ||
		MsgCmd::MsgCmd_PlayerCenter == identification ||
		MsgCmd::MsgCmd_Scene == identification)
	{
		CallBackFun(cmd, playerInfo);
		return;
	}
	PlayerClient* playerClient = G_PlayerCenterClient->GetPlayerByIndex(index);
	if (!playerClient)
	{
		Log(CERR, "Dispatch message playerClient = null index = %d", index);
		return;
	}
	const TCPSocketInfo* pInfo = G_NetClient->GetTCPSocketInfo(index);
	if (!pInfo)
	{
		Log(CERR, "Client information is empty index=%d", index);
		return;
	}
	if (!pInfo->isConnect)
	{
		Log(CINF, "Dispatch message Link broken cmd = %d", (int)cmd);
		return;
	}
	if (!playerClient->GetLoad())
	{
		Log(CERR, "Dispatch message mysql is unload index = %d", index);
		return;
	}
	if (playerClient->GetIndex() != index)
	{
		Log(CERR, "dindex = %u, sindex = %d", playerClient->GetIndex(), index);
		return;
	}
	playerClient->MessageDispatch(cmd, playerInfo);
}
void PlayerPrep::MessageCrossDispatch(PlayerInfo* playerInfo)
{
	if (!playerInfo)
	{
		Log(CERR, "!playerInfo");
		return;
	}
	SocketReadLine* pMsg = playerInfo->pMsg;
	if (!pMsg)
	{
		Log(CERR, "!pMsg");
		return;
	}
	int index = pMsg->uIndex;
	TCPSocketInfo* pTcpInfo = G_NetClient->GetTCPSocketInfo(index);
	if (!pTcpInfo)
	{
		Log(CERR, "!tcpInfo");
		return;
	}
	MsgCmd cmd = (MsgCmd)pMsg->netMessageHead.uMainID;
	if (cmd >= MsgCmd::MsgCmd_End || cmd <= MsgCmd::MsgCmd_Begin)
	{
		Log(CERR, "非法消息cmd=%d", (int)cmd);
		return;
	}
	MsgCmd identification = (MsgCmd)pMsg->netMessageHead.uIdentification;
	if (MsgCmd::MsgCmd_PlayerPreproces == identification ||
		MsgCmd::MsgCmd_PlayerCenter == identification ||
		MsgCmd::MsgCmd_Scene == identification)
	{
		CallBackFun(cmd, playerInfo);
		return;
	}
	
	Netmsg msg((char*)playerInfo->pData, 3);
	if (msg.size() < 2)
	{
		Log(CERR, "逻辑服务器发送过来的协议不对 size=%d", (int)(msg.size()));
		return;
	}
	int sid = 0;
	uint64_t userid = 0;
	msg >> sid >> userid;
	PlayerClient* playerClient = G_PlayerCenterClient->GetPlayerByUserid(userid);
	if (!playerClient)
	{
		Log(CERR, "Dispatch message playerClient = null index = %d", index);
		return;
	}
	if (!pTcpInfo->isConnect)
	{
		Log(CINF, "Dispatch message Link broken cmd = %d", (int)cmd);
		return;
	}
	if (!playerClient->GetLoad())
	{
		Log(CERR, "Dispatch message mysql is unload index = %d", index);
		return;
	}
	if (playerClient->GetIndex() != index)
	{
		Log(CERR, "dindex = %u, sindex = %d", playerClient->GetIndex(), index);
		return;
	}
	playerClient->MessageDispatch(cmd, playerInfo);
}
// 处理消息
void PlayerPrep::MessageDispatch(PlayerInfo* playerInfo)
{
	G_NetClient->GetServerType() ==
		ServiceType::SERVICE_TYPE_CROSS ?
		MessageCrossDispatch(playerInfo) :
		MessageLogicDispatch(playerInfo);
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