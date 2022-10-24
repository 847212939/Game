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
	Netmsg msg((char*)playerInfo->pData);

	switch (uAssistantID)
	{
	case DataBaseSysMsgCmd::cs_load_player:
	{
		LoadPlayerMysql(msg, playerInfo);
		break;
	}
	case DataBaseSysMsgCmd::cs_load_global:
	{
		LoadGlobalMysql(msg, playerInfo);
		break;
	}
	case DataBaseSysMsgCmd::cs_save_replace_player:
	{
		SaveReplacePlayerMysql(msg, playerInfo);
		break;
	}
	case DataBaseSysMsgCmd::cs_save_replace_global:
	{
		SaveReplaceGlobalMysql(msg, playerInfo);
		break;
	}
	case DataBaseSysMsgCmd::cs_create_global:
	{
		CreateGlobalMysql(msg, playerInfo);
		break;
	}
	case DataBaseSysMsgCmd::cs_create_player:
	{
		CreatePlayerMysql(msg, playerInfo);
		break;
	}
	case DataBaseSysMsgCmd::cs_load_login:
	{
		LoadLoginMysql(msg, playerInfo);
		break;
	}
	case DataBaseSysMsgCmd::cs_create_login:
	{
		CreateLoginMysql(msg, playerInfo);
		break;
	}
	default:
		break;
	}
}

bool DataBaseSys::CreateLoginMysql(Netmsg& msg, PlayerInfo* playerInfo)
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
	Netmsg msgCin((char*)playerInfo->pData);

	int cnt = 0;
	std::string sqlName;
	msgCin >> sqlName >> cnt;

	DPPC->CreateLoginTable(sqlName, cnt);

	return true;
}
bool DataBaseSys::CreateGlobalMysql(Netmsg& msg, PlayerInfo* playerInfo)
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
	Netmsg msgCin((char*)playerInfo->pData);

	int cnt = 0;
	std::string sqlName;
	msgCin >> sqlName >> cnt;

	DPPC->CreateGlobalTable(sqlName, cnt);

	return true;
}
bool DataBaseSys::CreatePlayerMysql(Netmsg& msg, PlayerInfo* playerInfo)
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
	Netmsg msgCin((char*)playerInfo->pData);

	int cnt = 0;
	std::string sqlName;
	msgCin >> sqlName >> cnt;

	DPPC->CreatePlayerTable(sqlName, cnt);

	return true;
}

bool DataBaseSys::LoadPlayerMysql(Netmsg& msg, PlayerInfo* playerInfo)
{
	if (!playerInfo)
	{
		return false;
	}
	if (!playerInfo->pMsg)
	{
		return false;
	}

	SocketReadLine* pMsg = playerInfo->pMsg;
	int serverid = 0;
	std::string sqlName;
	std::string outStr;
	uint64_t userid = 0;
	unsigned int uMainID = 0;
	unsigned int uAssistantID = 0;
	unsigned int uIdentification = 0;

	msg >> serverid >> userid >> sqlName >> uMainID >> uAssistantID >> uIdentification;
	DPPC->LoadPlayerMysql(sqlName, serverid, userid, outStr);

	Netmsg msgCout;
	msgCout << outStr;

	DTCPC->SendMsg(pMsg->uIndex, msgCout.str().c_str(), msgCout.str().size(), (MsgCmd)uMainID,
		uAssistantID, 0, pMsg->pBufferevent, uIdentification);

	return true;
}
bool DataBaseSys::LoadLoginMysql(Netmsg& msg, PlayerInfo* playerInfo)
{
	if (!playerInfo)
	{
		return false;
	}
	if (!playerInfo->pMsg)
	{
		return false;
	}

	SocketReadLine* pMsg = playerInfo->pMsg;
	int serverid = 0;
	int index = 0;
	std::string sqlName;
	std::string outStr;
	std::string userid;
	std::string passWord;
	unsigned int uMainID = 0;
	unsigned int uAssistantID = 0;
	unsigned int uIdentification = 0;

	msg >> serverid >> userid >> passWord >> index >> sqlName >> uMainID >> uAssistantID >> uIdentification;
	DPPC->LoadLoginMysql(sqlName, serverid, userid, outStr);

	Netmsg msgCout;
	msgCout << userid << passWord << index;
	if (!outStr.empty())
	{
		msgCout << outStr;
	}

	DTCPC->SendMsg(pMsg->uIndex, msgCout.str().c_str(), msgCout.str().size(), (MsgCmd)uMainID,
		uAssistantID, 0, pMsg->pBufferevent, uIdentification);

	return true;
}
bool DataBaseSys::LoadGlobalMysql(Netmsg& msg, PlayerInfo* playerInfo)
{
	if (!playerInfo)
	{
		return false;
	}
	if (!playerInfo->pMsg)
	{
		return false;
	}

	SocketReadLine* pMsg = playerInfo->pMsg;
	int serverid = 0;
	std::string sqlName;
	std::string outStr;
	unsigned int uMainID = 0;
	unsigned int uAssistantID = 0;
	unsigned int uIdentification = 0;

	msg >> serverid >> sqlName >> uMainID >> uAssistantID >> uIdentification;
	DPPC->LoadGlobalMysql(sqlName, serverid, outStr);

	Netmsg msgCout;
	msgCout << outStr;

	DTCPC->SendMsg(pMsg->uIndex, msgCout.str().c_str(), msgCout.str().size(), (MsgCmd)uMainID,
		uAssistantID, 0, pMsg->pBufferevent, uIdentification);

	return true;
}

bool DataBaseSys::SaveReplaceGlobalMysql(Netmsg& msg, PlayerInfo* playerInfo)
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
	std::string sqlName;
	std::string data;

	msg >> serverid >> sqlName >> data;
	DPPC->SaveReplaceSQL(sqlName, serverid, data);

	return true;
}
bool DataBaseSys::SaveReplacePlayerMysql(Netmsg& msg, PlayerInfo* playerInfo)
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
	std::string data;

	msg >> serverid >> userid >> sqlName >> data;
	DPPC->SaveReplaceSQL(sqlName, serverid, userid, data);

	return true;
}

// ±£´æ
bool DataBaseSys::SaveMysql(Netmsg& msg, PlayerInfo* playerInfo)
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