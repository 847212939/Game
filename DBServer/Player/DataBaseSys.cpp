#include "../stdafx.h"

DataBaseSys::DataBaseSys(PlayerPrepClient* ppc)
{
	RegisterNetwk(ppc, DataBaseSys::Network, MsgCmd::MsgCmd_DBServer);
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

	switch (uAssistantID)
	{
	case DataBaseSysMsgCmd::cs_load:
	{
		LoadMysql(playerInfo);
		break;
	}
	case DataBaseSysMsgCmd::cs_save:
	{
		SaveMysql(playerInfo);
		break;
	}
	default:
		break;
	}
}

// ¼ÓÔØ
bool DataBaseSys::LoadMysql(PlayerInfo* playerInfo)
{
	if (!playerInfo)
	{
		return false;
	}
	if (!playerInfo->pMsg)
	{
		return false;
	}

	int serverid = 0;
	uint64_t playerid = 0;
	std::string sqlName;

	Netmsg msgCin((char*)playerInfo->pData, 3);
	msgCin >> serverid >> playerid >> sqlName;

	std::string outStr;
	DPPC->LoadOneSql(sqlName, serverid, playerid, outStr);

	Netmsg msgCout;
	msgCout << sqlName << outStr;

	auto* pMsg = playerInfo->pMsg;
	DTCPC->SendMsg(pMsg->uIndex, msgCout.str().c_str(), msgCout.str().size(), (MsgCmd)pMsg->netMessageHead.uMainID, 
		pMsg->netMessageHead.uAssistantID, pMsg->netMessageHead.uHandleCode, pMsg->pBufferevent, (unsigned int)MsgCmd::MsgCmd_DBServer);

	return true;
}

// ±£´æ
bool DataBaseSys::SaveMysql(PlayerInfo* playerInfo)
{
	if (!playerInfo)
	{
		return false;
	}
	if (!playerInfo->pMsg)
	{
		return false;
	}
	Netmsg msg((char*)playerInfo->pData, 1);

	return true;
}