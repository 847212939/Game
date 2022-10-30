#include "../stdafx.h"

CrossClient::CrossClient(PlayerPrepClient* ppc)
{
	RegisterNetwk(ppc, CrossClient::Network, MsgCmd::MsgCmd_Cross);
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
	case CrossClientMsgCmd::cs_cross_login:
	{
		break;
	}
	default:
		break;
	}
}