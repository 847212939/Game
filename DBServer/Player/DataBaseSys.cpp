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
	Netmsg msg((char*)playerInfo->pData, playerInfo->pMsg->uHandleSize, 3);

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
	Netmsg msg((char*)playerInfo->pData, playerInfo->pMsg->uHandleSize, 3);

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
	Netmsg msg((char*)playerInfo->pData, playerInfo->pMsg->uHandleSize, 3);

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

	Netmsg msg((char*)playerInfo->pData, playerInfo->pMsg->uHandleSize, 8);

	// 1.要发送的协议
	unsigned int uMainID = 0;
	unsigned int uAssistantID = 0;
	unsigned int uIdentification = 0;
	msg >> uMainID
		>> uAssistantID
		>> uIdentification;

	// 2.玩家id，本服索引
	uint64_t userid = 0;
	unsigned int uIndex = 0;
	msg >> userid
		>> uIndex;

	// 3.服务器类型，服务器id
	int serverType = 0;
	int serverid = 0;
	msg >> serverType
		>> serverid;

	// 4.数据库名称
	std::string sqlName;
	msg >> sqlName;

	std::string outStr;
	G_PlayerPrepClient->LoadPlayerMysql(sqlName, serverid, userid, outStr);

	// 1.发送的数据
	msg << serverid;
	(ServiceType)serverType == ServiceType::SERVICE_TYPE_CROSS ?
	msg << userid:
	msg << uIndex;
	if (!outStr.empty())
	{
		msg << outStr;
	}

	G_NetClient->SendMsg(playerInfo->pMsg->uIndex, msg.str().c_str(), msg.str().size(), (MsgCmd)uMainID,
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
	Netmsg msg((char*)playerInfo->pData, playerInfo->pMsg->uHandleSize, 9);

	// 1.要发送的协议
	unsigned int uMainID = 0;
	unsigned int uAssistantID = 0;
	unsigned int uIdentification = 0;
	msg >> uMainID
		>> uAssistantID
		>> uIdentification;

	// 2.玩家账号密码，本服索引
	std::string userid;
	std::string passWord;
	unsigned int uIndex = 0;
	msg >> userid
		>> passWord
		>> uIndex;

	// 3.服务器类型，服务器id
	int serverType = 0;
	int serverid = 0;
	msg >> serverType
		>> serverid;

	// 4.数据库名称
	std::string sqlName;
	msg >> sqlName;

	std::string outStr;
	G_PlayerPrepClient->LoadLoginMysql(sqlName, serverid, userid, outStr);

	msg << serverid;
	(ServiceType)serverType == ServiceType::SERVICE_TYPE_CROSS ?
	msg << userid:
	msg << uIndex;
	msg << passWord;
	if (!outStr.empty())
	{
		msg << outStr;
	}

	G_NetClient->SendMsg(playerInfo->pMsg->uIndex, msg.str().c_str(), msg.str().size(), (MsgCmd)uMainID,
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
	Netmsg msg((char*)playerInfo->pData, playerInfo->pMsg->uHandleSize, 7);

	// 1.要发送的协议
	unsigned int uMainID = 0;
	unsigned int uAssistantID = 0;
	unsigned int uIdentification = 0;
	msg >> uMainID
		>> uAssistantID
		>> uIdentification;

	// 2.服务器类型，服务器id
	int serverType = 0;
	int serverid = 0;
	msg >> serverType
		>> serverid;

	// 3.数据库名称
	std::string sqlName;
	msg >> sqlName;

	// 4.本服索引
	unsigned int uIndex = 0;
	msg >> uIndex;

	std::string outStr;
	G_PlayerPrepClient->LoadGlobalMysql(sqlName, serverid, outStr);

	uint64_t userid = 0;
	msg << serverid;
	(ServiceType)serverType == ServiceType::SERVICE_TYPE_CROSS ?
		msg << userid:
		msg << uIndex;
	if (!outStr.empty())
	{
		msg << outStr;
	}

	G_NetClient->SendMsg(playerInfo->pMsg->uIndex, outStr.c_str(), outStr.size(), (MsgCmd)uMainID,
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
	Netmsg msg((char*)playerInfo->pData, playerInfo->pMsg->uHandleSize, 4);

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
	Netmsg msg((char*)playerInfo->pData, playerInfo->pMsg->uHandleSize, 5);

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
	Netmsg msg((char*)playerInfo->pData, playerInfo->pMsg->uHandleSize, 5);

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