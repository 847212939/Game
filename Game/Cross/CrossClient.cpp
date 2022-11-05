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

	msg << userid;
	msg << m_Player->GetAnimalid();
	msg << m_Player->GetRefreshTime();
	msg << m_Player->GetLived();
	msg << (int)m_Player->GetAnimaltype();
	msg << m_Player->GetAnimalname().c_str();
	msg << m_Player->GetPlayername().c_str();
	msg << G_CfgMgr->GetCBaseCfgMgr().GetServerId();
	msg << logciIndex;

	new(m_Player) PlayerClient(playerInfo->pMsg->uIndex);
	m_Player->SetID(userid);
	m_Player->SetIndex(playerInfo->pMsg->uIndex);

	G_NetClient->SendMsg(crossIndex, msg.str().c_str(), msg.str().size(), MsgCmd::MsgCmd_CrossLogin,
		(int)CrossClientMsgCmd::cs_logic_to_cross_login, 0, pCrossTcpInfo->bev, (unsigned int)MsgCmd::MsgCmd_PlayerPreproces, userid);

	Log(CINF, "userid=%lld logic->cross玩家请求跨服登陆", userid);

	return true;
}
// 客户端请求跨服断开链接
bool CrossClient::ClientToCrossLogout(Netmsg& msg, PlayerInfo* playerInfo)
{
	if (G_NetClient->GetServerType() != ServiceType::SERVICE_TYPE_CROSS)
	{
		return false;
	}
	
	TCPSocketInfo* pLogicTcpInfo = G_NetClient->GetTCPSocketInfo(playerInfo->pMsg->uIndex);
	if (!pLogicTcpInfo)
	{
		return false;
	}

	msg << m_Player->GetID();
	msg << m_Player->GetAnimalid();
	msg << m_Player->GetRefreshTime();
	msg << m_Player->GetLived();
	msg << (int)m_Player->GetAnimaltype();
	msg << m_Player->GetAnimalname().c_str();
	msg << m_Player->GetPlayername().c_str();
	msg << G_CfgMgr->GetCBaseCfgMgr().GetServerId();
	msg << m_Player->GetLogicIndex();

	G_NetClient->SendMsg(playerInfo->pMsg->uIndex, msg.str().c_str(), msg.str().size(),
		MsgCmd::MsgCmd_CrossLogin,(int)
		CrossClientMsgCmd::cs_cross_to_logic_logout, 0, pLogicTcpInfo->bev, (unsigned int)
		MsgCmd::MsgCmd_PlayerPreproces, m_Player->GetID());

	// 跨服删除玩家
	G_NetClient->OnSocketCloseEvent(0, 0, 0, true, m_Player->GetID());

	Log(CINF, "userid=%lld client->cross玩家请求跨服退出", m_Player->GetID());

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

	Log(CINF, "userid=%lld logic->cross网络断开请求跨服退出", m_Player->GetID());

	return true;
}
