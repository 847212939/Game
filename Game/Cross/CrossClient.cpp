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
	Netmsg msg((char*)playerInfo->pData);

	switch (uAssistantID)
	{
	case CrossClientMsgCmd::cs_logic_to_cross_login:
	{
		LogicToCrossLogin(msg, playerInfo);
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

	Netmsg msgCin;
	msgCin << m_Player->GetID();
	msgCin << m_Player->GetAnimalid();
	msgCin << m_Player->GetRefreshTime();
	msgCin << m_Player->GetLived();
	msgCin << (int)m_Player->GetAnimaltype();
	msgCin << m_Player->GetAnimalname().c_str();
	msgCin << m_Player->GetPlayername().c_str();
	msgCin << G_CfgMgr->GetCBaseCfgMgr().GetServerId();
	msgCin << playerInfo->pMsg->uIndex;

	G_NetClient->SendMsg(crossIndex, msgCin.str().c_str(), msgCin.str().size(), MsgCmd::MsgCmd_CrossLogin,
		(int)CrossClientMsgCmd::cs_logic_to_cross_login, 0, pCrossTcpInfo->bev, (unsigned int)MsgCmd::MsgCmd_PlayerPreproces);

	SafeDelete(m_Player);
	m_Player = new PlayerClient(playerInfo->pMsg->uIndex);
	VectorPlayerClient& playerClientVec = G_PlayerCenterClient->GetVectorPlayerClient();
	playerClientVec[playerInfo->pMsg->uIndex] = nullptr;
	playerClientVec[playerInfo->pMsg->uIndex] = m_Player;

	return true;
}

void CrossClient::LogoutCross()
{
	Log(CINF, "ÍË³ö¿ç·þ");
}