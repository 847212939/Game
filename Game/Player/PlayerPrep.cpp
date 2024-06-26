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

bool PlayerPrep::MessageLogicMachin(int& index, PlayerInfo* playerInfo, TCPSocketInfo* tcpInfo)
{
	if (!G_NetClient->IsServerIndex(index))
	{
		if (tcpInfo->link != (uint64_t)MsgCmd::MsgCmd_Testlink)
		{
			G_NetClient->CloseSocket(index);
			return false;
		}
	}
	else
	{
		if (G_NetClient->GetCrossServerIndex() == index)
		{
			Netmsg cin((char*)playerInfo->pData, playerInfo->pMsg->uHandleSize, 2);
			if (cin.is_size() < 1)
			{
				return false;
			}
			unsigned int uIndex = 0;
			std::string dataMsg;
			cin >> uIndex
				>> dataMsg;

			index = (int)uIndex;

			playerInfo->serMsgData.uIndex = uIndex;

			memcpy(playerInfo->pData, dataMsg.c_str(), dataMsg.size());
			playerInfo->pMsg->uHandleSize = (unsigned int)dataMsg.size();
		}
		else if (G_NetClient->GetDBServerIndex() == index)
		{
			Netmsg cin((char*)playerInfo->pData, playerInfo->pMsg->uHandleSize, 3);
			if (cin.is_size() < 2)
			{
				return false;
			}
			int serverid = 0;
			unsigned int uIndex = 0;
			std::string dataMsg;
			cin >> serverid
				>> uIndex
				>> dataMsg;

			index = (int)uIndex;

			playerInfo->serMsgData.serverid = serverid;
			playerInfo->serMsgData.uIndex = uIndex;

			memcpy(playerInfo->pData, dataMsg.c_str(), dataMsg.size());
			playerInfo->pMsg->uHandleSize = (unsigned int)dataMsg.size();
		}
	}

	return true;
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
	TCPSocketInfo* pSerTcpInfo = G_NetClient->GetTCPSocketInfo(index);
	if (!pSerTcpInfo)
	{
		Log(CERR, "!pSerTcpInfo");
		return;
	}
	if (cmd >= MsgCmd::MsgCmd_End || cmd <= MsgCmd::MsgCmd_Begin)
	{
		Log(CERR, "非法消息[cmd=%d,index=%d,ip=%s,port=%d]",
			(int)cmd, index, pSerTcpInfo->ip, pSerTcpInfo->port);
		return;
	}
	if (!MessageLogicMachin(index, playerInfo, pSerTcpInfo))
	{
		Log(CERR, "非法消息[cmd=%d,index=%d,ip=%s,port=%d]",
			(int)cmd, index, pSerTcpInfo->ip, pSerTcpInfo->port);
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
	const TCPSocketInfo* pClientTcpInfo = G_NetClient->GetTCPSocketInfo(index);
	if (!pClientTcpInfo)
	{
		Log(CERR, "!pClientTcpInfo");
		return;
	}
	if (!pClientTcpInfo->isConnect)
	{
		Log(CERR, "!pClientTcpInfo->isConnect[cmd=%d,index=%d,ip=%s,port=%d]",
			(int)cmd, index, pClientTcpInfo->ip, pClientTcpInfo->port);
		return;
	}
	PlayerClient* playerClient = G_PlayerCenterClient->GetPlayerByIndex(index);
	if (!playerClient)
	{
		Log(CERR, "!playerClient[cmd=%d,index=%d,ip=%s,port=%d]", 
			(int)cmd, index, pSerTcpInfo->ip, pSerTcpInfo->port);
		return;
	}
	if (!playerClient->GetLoad())
	{
		Log(CERR, "!playerClient->GetLoad()[cmd=%d,index=%d,ip=%s,port=%d]",
			(int)cmd, index, pClientTcpInfo->ip, pClientTcpInfo->port);
		return;
	}
	if (playerClient->GetIndex() != index && index != G_NetClient->GetDBServerIndex())
	{
		Log(CERR, "playerClient->GetIndex() != index[cmd=%d,index=%d,ip=%s,port=%d]",
			(int)cmd, index, pClientTcpInfo->ip, pClientTcpInfo->port);
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

	Netmsg cin((char*)playerInfo->pData, playerInfo->pMsg->uHandleSize, 3);
	if (cin.is_size() < 2)
	{
		Log(CERR, "非法消息[cmd=%d,index=%d,ip=%s,port=%d]",
			(int)cmd, index, pTcpInfo->ip, pTcpInfo->port);
		return;
	}

	int serverid = 0;
	uint64_t userid = 0;
	std::string dataMsg;
	cin >> serverid
		>> userid
		>> dataMsg;

	memcpy(playerInfo->pData, dataMsg.c_str(), dataMsg.size());
	playerInfo->pMsg->uHandleSize = (unsigned int)dataMsg.size();

	MsgCmd identification = (MsgCmd)pMsg->netMessageHead.uIdentification;
	if (MsgCmd::MsgCmd_PlayerPreproces == identification ||
		MsgCmd::MsgCmd_PlayerCenter == identification ||
		MsgCmd::MsgCmd_Scene == identification)
	{
		CallBackFun(cmd, playerInfo);
		return;
	}

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
	G_NetClient->GetServerType() == ServiceType::SERVICE_TYPE_CROSS ?
	MessageCrossDispatch(playerInfo):
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
		m_NetCBFunMap.emplace(std::make_pair(cmd, fun));
		return;
	}

	Log(CERR, "网络回调添加错误 cmd = %d", cmd);
}
bool PlayerPrep::CallBackFun(MsgCmd cmd, PlayerInfo* playerInfo)
{
	MapNetFun::iterator it = m_NetCBFunMap.find(cmd);
	if (it == m_NetCBFunMap.end())
	{
		Log(CERR, "网络回调错误 cmd = %d", cmd);
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
		m_TimerFunMap.emplace(std::make_pair(cmd, fun));
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