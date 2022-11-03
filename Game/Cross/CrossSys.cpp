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
		CrossLogin(msg, playerInfo);
		break;
	}
	case CrossClientMsgCmd::cs_logic_to_cross_logout:
	{
		CrossLogout(msg, playerInfo);
		break;
	}
	default:
		break;
	}
}

bool CrossSys::CrossLogin(Netmsg& msg, PlayerInfo* playerInfo)
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

	return true;
}

bool CrossSys::CrossLogout(Netmsg& msg, PlayerInfo* playerInfo)
{
	uint64_t userid = 0;
	msg >> userid;

	// 投递到关闭回调函数中
	G_NetClient->OnSocketCloseEvent(0, 0, 0, true, userid);

	return true;
}