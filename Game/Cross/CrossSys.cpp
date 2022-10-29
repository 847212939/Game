#include "../stdafx.h"

CrossSys::CrossSys(PlayerPrepClient* ppc)
{
	RegisterNetwk(ppc, CrossSys::Network, MsgCmd::MsgCmd_Cross);
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
	Netmsg msg((char*)playerInfo->pData);

	switch (uAssistantID)
	{
	case CrossClientMsgCmd::cs_logic_to_cross_login:
	{
		CrossLogin(msg, playerInfo);
		break;
	}
	default:
		break;
	}
}

bool CrossSys::CrossLogin(Netmsg& msg, PlayerInfo* playerInfo)
{
	int serverid = 0;
	uint64_t userid = 0;

	msg >> serverid >> userid;

	//G_PlayerCenterClient->CreatePlayer();
	return true;
}