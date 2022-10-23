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
	case DataBaseSysMsgCmd::cs_create:
	{
		CreateMysql(playerInfo);
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
	uint64_t userid = 0;
	std::string sqlName;

	Netmsg msgCin((char*)playerInfo->pData, 4);
	msgCin >> serverid >> userid >> sqlName;

	std::string outStr;
	DPPC->LoadOneSql(sqlName, serverid, userid, outStr);

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
	int serverid = 0;
	uint64_t userid = 0;
	std::string sqlName, data;

	Netmsg msgCin((char*)playerInfo->pData, 4);
	msgCin >> serverid >> userid >> sqlName >> data;

	DPPC->SaveReplaceSQL(sqlName, serverid, userid, data);

	return true;
}

bool DataBaseSys::CreateMysql(PlayerInfo* playerInfo)
{
	if (!playerInfo)
	{
		return false;
	}
	if (!playerInfo->pMsg)
	{
		return false;
	}

	auto* pMsg = playerInfo->pMsg;
	Netmsg msgCin((char*)playerInfo->pData, 4);

	int cnt = 0, option = 0;
	std::string sqlName;
	msgCin >> option >> sqlName >> cnt;

	if (option == 1)
	{
		DPPC->CreateTableS(sqlName, cnt);
	}
	else if (option == 2)
	{
		DPPC->CreateTableI(sqlName, cnt);
	}

	return true;
}