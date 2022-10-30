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
		Log(CERR, "Player Info is null");
		return;
	}
	if (!playerInfo->pMsg)
	{
		Log(CERR, "Player Info pMsg is null");
		return;
	}

	DataBaseSysMsgCmd uAssistantID = (DataBaseSysMsgCmd)playerInfo->pMsg->netMessageHead.uAssistantID;

	switch (uAssistantID)
	{
	case DataBaseSysMsgCmd::cs_load_player:
	{
		LoadPlayerMysql(playerInfo);
		break;
	}
	case DataBaseSysMsgCmd::cs_load_global:
	{
		LoadGlobalMysql(playerInfo);
		break;
	}
	case DataBaseSysMsgCmd::cs_load_login:
	{
		LoadLoginMysql(playerInfo);
		break;
	}
	case DataBaseSysMsgCmd::cs_save_replace_player:
	{
		SaveReplacePlayerMysql(playerInfo);
		break;
	}
	case DataBaseSysMsgCmd::cs_save_replace_global:
	{
		SaveReplaceGlobalMysql(playerInfo);
		break;
	}
	case DataBaseSysMsgCmd::cs_save_replace_login:
	{
		SaveReplaceLoginMysql(playerInfo);
		break;
	}
	case DataBaseSysMsgCmd::cs_create_global:
	{
		CreateGlobalMysql(playerInfo);
		break;
	}
	case DataBaseSysMsgCmd::cs_create_player:
	{
		CreatePlayerMysql(playerInfo);
		break;
	}
	case DataBaseSysMsgCmd::cs_create_login:
	{
		CreateLoginMysql(playerInfo);
		break;
	}
	default:
		break;
	}
}

bool DataBaseSys::CreateLoginMysql(PlayerInfo* playerInfo)
{
	if (!playerInfo)
	{
		return false;
	}
	if (!playerInfo->pMsg)
	{
		return false;
	}
	Netmsg msg((char*)playerInfo->pData, 2);

	int serverType = 0;
	int cnt = 0;
	std::string sqlName;
	msg >> serverType
		>> sqlName 
		>> cnt;

	if ((ServiceType)serverType == ServiceType::SERVICE_TYPE_CROSS)
	{
		return true;
	}

	G_PlayerPrepClient->CreateLoginTable(sqlName, cnt);

	return true;
}
bool DataBaseSys::CreateGlobalMysql(PlayerInfo* playerInfo)
{
	if (!playerInfo)
	{
		return false;
	}
	if (!playerInfo->pMsg)
	{
		return false;
	}
	Netmsg msg((char*)playerInfo->pData, 2);

	int serverType = 0;
	int cnt = 0;
	std::string sqlName;
	msg >> serverType
		>> sqlName 
		>> cnt;

	if ((ServiceType)serverType == ServiceType::SERVICE_TYPE_CROSS)
	{
		return true;
	}

	G_PlayerPrepClient->CreateGlobalTable(sqlName, cnt);

	return true;
}
bool DataBaseSys::CreatePlayerMysql(PlayerInfo* playerInfo)
{
	if (!playerInfo)
	{
		return false;
	}
	if (!playerInfo->pMsg)
	{
		return false;
	}
	Netmsg msg((char*)playerInfo->pData, 2);

	int serverType = 0;
	int cnt = 0;
	std::string sqlName;
	msg >> serverType
		>> sqlName 
		>> cnt;

	if ((ServiceType)serverType == ServiceType::SERVICE_TYPE_CROSS)
	{
		return true;
	}

	G_PlayerPrepClient->CreatePlayerTable(sqlName, cnt);

	return true;
}

bool DataBaseSys::LoadPlayerMysql(PlayerInfo* playerInfo)
{
	if (!playerInfo)
	{
		return false;
	}
	if (!playerInfo->pMsg)
	{
		return false;
	}
	Netmsg msg((char*)playerInfo->pData, 6);

	int serverType = 0;
	int serverid = 0;
	std::string sqlName;
	std::string outStr;
	uint64_t userid = 0;
	unsigned int uMainID = 0;
	unsigned int uAssistantID = 0;
	unsigned int uIdentification = 0;

	msg >> serverType
		>> serverid 
		>> userid 
		>> sqlName 
		>> uMainID 
		>> uAssistantID 
		>> uIdentification;

	G_PlayerPrepClient->LoadPlayerMysql(sqlName, serverid, userid, outStr);

	Netmsg msgCout;
	msgCout 
		<< serverid 
		<< userid 
		<< outStr;

	G_NetClient->SendMsg(playerInfo->pMsg->uIndex, msgCout.str().c_str(), msgCout.str().size(), (MsgCmd)uMainID,
		uAssistantID, 0, playerInfo->pMsg->pBufferevent, uIdentification);

	return true;
}
bool DataBaseSys::LoadLoginMysql(PlayerInfo* playerInfo)
{
	if (!playerInfo)
	{
		return false;
	}
	if (!playerInfo->pMsg)
	{
		return false;
	}
	Netmsg msg((char*)playerInfo->pData, 8);

	int serverType = 0;
	int serverid = 0;
	int index = 0;
	std::string sqlName;
	std::string outStr;
	std::string userid;
	std::string passWord;
	unsigned int uMainID = 0;
	unsigned int uAssistantID = 0;
	unsigned int uIdentification = 0;

	msg >> serverType
		>> serverid 
		>> userid 
		>> passWord 
		>> index 
		>> sqlName 
		>> uMainID 
		>> uAssistantID 
		>> uIdentification;

	G_PlayerPrepClient->LoadLoginMysql(sqlName, serverid, userid, outStr);

	Netmsg msgCout;
	msgCout 
		<< userid 
		<< passWord 
		<< index;

	if (!outStr.empty())
	{
		msgCout << outStr;
	}

	G_NetClient->SendMsg(playerInfo->pMsg->uIndex, msgCout.str().c_str(), msgCout.str().size(), (MsgCmd)uMainID,
		uAssistantID, 0, playerInfo->pMsg->pBufferevent, uIdentification);

	return true;
}
bool DataBaseSys::LoadGlobalMysql(PlayerInfo* playerInfo)
{
	if (!playerInfo)
	{
		return false;
	}
	if (!playerInfo->pMsg)
	{
		return false;
	}
	Netmsg msg((char*)playerInfo->pData, 5);

	int serverType = 0;
	int serverid = 0;
	std::string sqlName;
	std::string outStr;
	unsigned int uMainID = 0;
	unsigned int uAssistantID = 0;
	unsigned int uIdentification = 0;

	msg >> serverType
		>> serverid 
		>> sqlName 
		>> uMainID 
		>> uAssistantID 
		>> uIdentification;
	G_PlayerPrepClient->LoadGlobalMysql(sqlName, serverid, outStr);

	Netmsg msgCout;
	msgCout << outStr;

	G_NetClient->SendMsg(playerInfo->pMsg->uIndex, msgCout.str().c_str(), msgCout.str().size(), (MsgCmd)uMainID,
		uAssistantID, 0, playerInfo->pMsg->pBufferevent, uIdentification);

	return true;
}

bool DataBaseSys::SaveReplaceGlobalMysql(PlayerInfo* playerInfo)
{
	if (!playerInfo)
	{
		return false;
	}
	if (!playerInfo->pMsg)
	{
		return false;
	}
	Netmsg msg((char*)playerInfo->pData, 3);

	int serverType = 0;
	int serverid = 0;
	std::string sqlName;
	std::string data;

	msg >> serverType
		>> serverid 
		>> sqlName 
		>> data;
	G_PlayerPrepClient->SaveReplaceGlobalMysql(sqlName, serverid, data);

	return true;
}
bool DataBaseSys::SaveReplacePlayerMysql(PlayerInfo* playerInfo)
{
	if (!playerInfo)
	{
		return false;
	}
	if (!playerInfo->pMsg)
	{
		return false;
	}
	Netmsg msg((char*)playerInfo->pData, 4);

	int serverType = 0;
	int serverid = 0;
	uint64_t userid = 0;
	std::string sqlName;
	std::string data;

	msg >> serverType
		>> serverid 
		>> userid 
		>> sqlName 
		>> data;

	G_PlayerPrepClient->SaveReplacePlayerMysql(sqlName, serverid, userid, data);

	return true;
}
bool DataBaseSys::SaveReplaceLoginMysql(PlayerInfo* playerInfo)
{
	if (!playerInfo)
	{
		return false;
	}
	if (!playerInfo->pMsg)
	{
		return false;
	}
	Netmsg msg((char*)playerInfo->pData, 4);

	int serverType = 0;
	int serverid = 0;
	std::string userid;
	std::string sqlName;
	std::string data;

	msg >> serverType
		>> serverid 
		>> userid 
		>> sqlName 
		>> data;

	G_PlayerPrepClient->SaveReplaceLoginMysql(sqlName, serverid, userid, data);

	return true;
}