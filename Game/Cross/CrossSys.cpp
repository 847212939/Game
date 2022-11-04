#include "../stdafx.h"

CrossSys::CrossSys(PlayerPrepClient* ppc)
{
	RegisterNetwk(ppc, CrossSys::Network, MsgCmd::MsgCmd_CrossLogin);
}

CrossSys::~CrossSys()
{
}

void CrossSys::Network(PlayerInfo* playerInfo)
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
	case CrossClientMsgCmd::cs_logic_to_cross_logout:
	{
		LogicToCrossLogout(msg, playerInfo);
		break;
	}
	case CrossClientMsgCmd::cs_logic_to_cross_close:
	{
		CloseCross(msg, playerInfo);
		break;
	}
	default:
		break;
	}
}

bool CrossSys::LogicToCrossLogin(Netmsg& msg, PlayerInfo* playerInfo)
{
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

	LoginData loginData;
	loginData.index = playerInfo->pMsg->uIndex;
	loginData.roleName = animalname;
	loginData.netName = playername;
	loginData.userId = userid;
	loginData.roleid = animalid;
	loginData.roleType = animaltype;
	loginData.serverId = serverid;
	loginData.logicIndex = logicIndex;

	G_PlayerCenterClient->CreatePlayer(loginData);

	Log(CINF, "userid=%lld的玩家登录跨服", userid);

	return true;
}

bool CrossSys::LogicToCrossLogout(Netmsg& msg, PlayerInfo* playerInfo)
{
	int serverid = 0;
	uint64_t userid = 0;
	msg >> serverid
		>> userid;

	PlayerClient* player = G_PlayerCenterClient->GetPlayerByUserid(userid);
	if (!player)
	{
		return false;
	}
	TCPSocketInfo* pLogicTcpInfo = G_NetClient->GetTCPSocketInfo(playerInfo->pMsg->uIndex);
	if (!pLogicTcpInfo)
	{
		return false;
	}

	Netmsg msgCin;
	msgCin << userid;
	msgCin << player->GetAnimalid();
	msgCin << player->GetRefreshTime();
	msgCin << player->GetLived();
	msgCin << (int)player->GetAnimaltype();
	msgCin << player->GetAnimalname().c_str();
	msgCin << player->GetPlayername().c_str();
	msgCin << G_CfgMgr->GetCBaseCfgMgr().GetServerId();
	msgCin << player->GetLogicIndex();

	G_NetClient->SendMsg(playerInfo->pMsg->uIndex, msgCin.str().c_str(), msgCin.str().size(), MsgCmd::MsgCmd_CrossLogin,
		(int)CrossClientMsgCmd::cs_cross_to_logic_logout, 0, pLogicTcpInfo->bev, 0, userid);
	
	return true;
}

bool CrossSys::CloseCross(Netmsg& msg, PlayerInfo* playerInfo)
{
	uint64_t userid = 0;
	msg >> userid;

	// 投递到关闭回调函数中
	G_NetClient->OnSocketCloseEvent(0, 0, 0, true, userid);

	Log(CINF, "userid=%lld的玩家网络断开被迫退出跨服", userid);
	return true;
}