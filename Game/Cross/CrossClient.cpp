#include "../stdafx.h"

CrossClient::CrossClient(PlayerClient* player) : m_Player(player)
{
	RegisterNetwk(player, CrossClient::Network, MsgCmd::MsgCmd_LoginCross);
}

CrossClient::~CrossClient()
{
}

void CrossClient::Network(PlayerInfo* playerInfo)
{
	if (!playerInfo)
	{
		Log(CERR, "Player Info is null");
		return;
	}
	if (!playerInfo->pMsg)
	{
		Log(CERR, "Player Info pMsg is null");
		return;
	}

	CrossClientMsgCmd uAssistantID = (CrossClientMsgCmd)playerInfo->pMsg->netMessageHead.uAssistantID;
	Netmsg msg((char*)playerInfo->pData, playerInfo->pMsg->uHandleSize, 0);

	switch (uAssistantID)
	{
	case CrossClientMsgCmd::cs_logic_to_cross_login:
	{
		LogicToCrossLogin(msg, playerInfo);
		break;
	}
	case CrossClientMsgCmd::cs_client_to_cross_logout:
	{
		ClientToCrossLogout(msg, playerInfo);
		break;
	}
	case CrossClientMsgCmd::cs_cross_to_logic_logout:
	{
		CrossToLogicLogout(msg, playerInfo);
		break;
	}
	default:
		break;
	}
}

bool CrossClient::LogicToCrossLogin(Netmsg& msg, PlayerInfo* playerInfo)
{
	int crossIndex = G_NetClient->GetCrossServerIndex();
	if (crossIndex < 0)
	{
		return false;
	}
	TCPSocketInfo* pCrossTcpInfo = G_NetClient->GetTCPSocketInfo(crossIndex);
	if (!pCrossTcpInfo)
	{
		return false;
	}
	TCPSocketInfo* pLogicTcpInfo = G_NetClient->GetTCPSocketInfo(playerInfo->pMsg->uIndex);
	if (!pLogicTcpInfo)
	{
		return false;
	}
	pLogicTcpInfo->isCross = true;
	uint64_t userid = m_Player->GetID();
	unsigned int logciIndex = playerInfo->pMsg->uIndex;

	Netmsg msgCin;
	msgCin << userid;
	msgCin << m_Player->GetAnimalid();
	msgCin << m_Player->GetRefreshTime();
	msgCin << m_Player->GetLived();
	msgCin << (int)m_Player->GetAnimaltype();
	msgCin << m_Player->GetAnimalname().c_str();
	msgCin << m_Player->GetPlayername().c_str();
	msgCin << G_CfgMgr->GetCBaseCfgMgr().GetServerId();
	msgCin << logciIndex;

	new(m_Player) PlayerClient(playerInfo->pMsg->uIndex);
	m_Player->SetID(userid);
	m_Player->SetIndex(playerInfo->pMsg->uIndex);

	G_NetClient->SendMsg(crossIndex, msgCin.str().c_str(), msgCin.str().size(), MsgCmd::MsgCmd_CrossLogin,
		(int)CrossClientMsgCmd::cs_logic_to_cross_login, 0, pCrossTcpInfo->bev, (unsigned int)MsgCmd::MsgCmd_PlayerPreproces, userid);

	return true;
}
// 客户端请求跨服断开链接
bool CrossClient::ClientToCrossLogout(Netmsg& msg, PlayerInfo* playerInfo)
{
	if (G_NetClient->GetServerType() != ServiceType::SERVICE_TYPE_CROSS)
	{
		return;
	}
	int serverid = 0;
	uint64_t userid = 0;
	msg >> serverid
		>> userid;

	TCPSocketInfo* pLogicTcpInfo = G_NetClient->GetTCPSocketInfo(playerInfo->pMsg->uIndex);
	if (!pLogicTcpInfo)
	{
		return false;
	}

	Netmsg msgCin;
	msgCin << userid;
	msgCin << m_Player->GetAnimalid();
	msgCin << m_Player->GetRefreshTime();
	msgCin << m_Player->GetLived();
	msgCin << (int)m_Player->GetAnimaltype();
	msgCin << m_Player->GetAnimalname().c_str();
	msgCin << m_Player->GetPlayername().c_str();
	msgCin << G_CfgMgr->GetCBaseCfgMgr().GetServerId();
	msgCin << m_Player->GetLogicIndex();

	G_NetClient->SendMsg(playerInfo->pMsg->uIndex, msgCin.str().c_str(), msgCin.str().size(), MsgCmd::MsgCmd_CrossLogin,
		(int)CrossClientMsgCmd::cs_cross_to_logic_logout, 0, pLogicTcpInfo->bev, 0, userid);

	// 跨服删除玩家
	G_NetClient->OnSocketCloseEvent(0, 0, 0, true, userid);

	return true;
}
// 跨服返回本服数据
bool CrossClient::CrossToLogicLogout(Netmsg& msg, PlayerInfo* playerInfo)
{
	if (G_NetClient->GetServerType() == ServiceType::SERVICE_TYPE_CROSS)
	{
		return;
	}
	uint64_t userid = 0;
	int animalid = 0;
	time_t refreshTime = 0;
	bool lived = false;
	int animaltype = 0;
	std::string animalname;
	std::string playername;
	int serverid;
	unsigned int logicIndex;

	msg >> userid
		>> animalid
		>> refreshTime
		>> lived
		>> animaltype
		>> animalname
		>> playername
		>> serverid
		>> logicIndex;

	TCPSocketInfo* pClientTcpInfo = G_NetClient->GetTCPSocketInfo(logicIndex);
	if (!pClientTcpInfo)
	{
		return false;
	}

	pClientTcpInfo->isCross = false;

	LoginData loginData;
	loginData.index = logicIndex;
	loginData.roleName = animalname;
	loginData.netName = playername;
	loginData.userId = userid;
	loginData.roleid = animalid;
	loginData.roleType = animaltype;
	loginData.serverId = serverid;
	loginData.logicIndex = 0;

	// 创建玩家
	G_PlayerCenterClient->CreatePlayer(loginData);

	Log(CINF, "userid=%lld的玩家退出跨服", userid);
	return true;
}
// 系统主动放松断开链接
bool CrossClient::CloseCross()
{
	int crossIndex = G_NetClient->GetCrossServerIndex();
	if (crossIndex < 0)
	{
		return false;
	}
	TCPSocketInfo* pCrossTcpInfo = G_NetClient->GetTCPSocketInfo(crossIndex);
	if (!pCrossTcpInfo)
	{
		return false;
	}
	uint64_t userid = m_Player->GetID();
	Netmsg msg;
	msg << userid;

	G_NetClient->SendMsg(crossIndex, msg.str().c_str(), msg.str().size(),
		MsgCmd::MsgCmd_CrossLogin, (int)CrossClientMsgCmd::cs_logic_to_cross_close,
		0, pCrossTcpInfo->bev, (unsigned int)MsgCmd::MsgCmd_PlayerPreproces, userid);

	return true;
}
