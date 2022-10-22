#include "../stdafx.h"

DataBaseSys::DataBaseSys(PlayerPrepClient* ppc)
{
	RegisterNetwk(ppc, DataBaseSys::Network, MsgCmd::MsgCmd_Login);
}

DataBaseSys::~DataBaseSys()
{
}

void DataBaseSys::Network(PlayerInfo* playerInfo)
{
	if (!playerInfo)
	{
		COUT_LOG(LOG_CERROR, "Player Info is null");
		return;
	}
	if (!playerInfo->pMsg)
	{
		COUT_LOG(LOG_CERROR, "Player Info pMsg is null");
		return;
	}

	DataBaseSysMsgCmd uAssistantID = (DataBaseSysMsgCmd)playerInfo->pMsg->netMessageHead.uAssistantID;
	Netmsg msg((char*)playerInfo->pData);

	switch (uAssistantID)
	{
	case DataBaseSysMsgCmd::cs_load:
	{
		break;
	}
	case DataBaseSysMsgCmd::cs_save:
	{
		break;
	}
	default:
		break;
	}
}