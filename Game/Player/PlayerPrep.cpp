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
	MsgCmd cmd = (MsgCmd)pMsg->netMessageHead.uMainID;
	TCPSocketInfo* tcpInfo = G_NetClient->GetTCPSocketInfo(index);
	if (!tcpInfo)
	{
		Log(CERR, "!tcpInfo");
		return;
	}
	if (!G_NetClient->IsServerIndex(index))
	{
		if (tcpInfo->link != (uint64_t)MsgCmd::MsgCmd_Testlink)
		{
			Log(CERR, "!tcpInfo->link != (uint64_t)MsgCmd::MsgCmd_Testlink,"
				"[cmd=%d,index=%d,ip=%s,port=%d]", (int)cmd, index, tcpInfo->ip, tcpInfo->port);
			G_NetClient->CloseSocket(index);
			return;
		}
	}
	if (cmd >= MsgCmd::MsgCmd_End || cmd <= MsgCmd::MsgCmd_Begin)
	{
		Log(CERR, "非法消息[cmd=%d,index=%d,ip=%s,port=%d]", 
			(int)cmd, index, tcpInfo->ip, tcpInfo->port);
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
	if (G_NetClient->GetDBServerIndex() == index)
	{
		Netmsg cin((char*)playerInfo->pData, playerInfo->pMsg->uHandleSize, 3);
		if (cin.size() < 2)
		{
			Log(CERR, "非法消息[cmd=%d,index=%d,ip=%s,port=%d]",
				(int)cmd, index, tcpInfo->ip, tcpInfo->port);
			return;
		}
		int serverid = 0;
		unsigned int uIndex = 0;
		cin >> serverid
			>> uIndex;

		index = (int)uIndex;
	}
	PlayerClient* playerClient = G_PlayerCenterClient->GetPlayerByIndex(index);
	if (!playerClient)
	{
		Log(CERR, "!playerClient[cmd=%d,index=%d,ip=%s,port=%d]", 
			(int)cmd, index, tcpInfo->ip, tcpInfo->port);
		return;
	}
	const TCPSocketInfo* pInfo = G_NetClient->GetTCPSocketInfo(index);
	if (!pInfo)
	{
		Log(CERR, "!pInfo[cmd=%d,index=%d,ip=%s,port=%d]",
			(int)cmd, index, tcpInfo->ip, tcpInfo->port);
		return;
	}
	if (!pInfo->isConnect)
	{
		Log(CERR, "!pInfo->isConnect[cmd=%d,index=%d,ip=%s,port=%d]",
			(int)cmd, index, tcpInfo->ip, tcpInfo->port);
		return;
	}
	if (!playerClient->GetLoad())
	{
		Log(CERR, "!playerClient->GetLoad()[cmd=%d,index=%d,ip=%s,port=%d]",
			(int)cmd, index, tcpInfo->ip, tcpInfo->port);
		return;
	}
	if (playerClient->GetIndex() != index && index != G_NetClient->GetDBServerIndex())
	{
		Log(CERR, "playerClient->GetIndex() != index[cmd=%d,index=%d,ip=%s,port=%d]",
			(int)cmd, index, tcpInfo->ip, tcpInfo->port);
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
	Netmsg cin((char*)playerInfo->pData, playerInfo->pMsg->uHandleSize, 3);
	if (cin.size() < 2)
	{
		Log(CERR, "非法消息[cmd=%d,index=%d,ip=%s,port=%d]",
			(int)cmd, index, pTcpInfo->ip, pTcpInfo->port);
		return;
	}
	int serverid = 0;
	uint64_t userid = 0;
	cin >> serverid
		>> userid;
	PlayerClient* playerClient = G_PlayerCenterClient->GetPlayerByUserid(userid);
	if (!playerClient)
	{
		Log(CERR, "!playerClient[cmd=%d,index=%d,ip=%s,port=%d,userid=%lld]",
			(int)cmd, index, pTcpInfo->ip, pTcpInfo->port, userid);
		return;
	}
	if (!pTcpInfo->isConnect)
	{
		Log(CERR, "!pTcpInfo->isConnect[cmd=%d,index=%d,ip=%s,port=%d,userid=%lld]",
			(int)cmd, index, pTcpInfo->ip, pTcpInfo->port, userid);
		return;
	}
	if (!playerClient->GetLoad())
	{
		Log(CERR, "!playerClient->GetLoad()[cmd=%d,index=%d,ip=%s,port=%d,userid=%lld]",
			(int)cmd, index, pTcpInfo->ip, pTcpInfo->port, userid);
		return;
	}
	if (playerClient->GetIndex() != index && index != G_NetClient->GetDBServerIndex())
	{
		Log(CERR, "!playerClient->GetIndex() != index && index != G_NetClient->GetDBServerIndex()"
			"[cmd=%d,index=%d,ip=%s,port=%d,userid=%lld]", (int)cmd, index, pTcpInfo->ip, pTcpInfo->port, userid);
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