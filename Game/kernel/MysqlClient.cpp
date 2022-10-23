#include "../stdafx.h"

MysqlClient::MysqlClient()
{
}

MysqlClient::~MysqlClient()
{
}

void MysqlClient::LoadPlayerMysql(Player* player, SLoadMysql& loadMysql)
{
	if (!player)
	{
		COUT_LOG(LOG_CERROR, "player null");
		return;
	}
	int index = DTCPC->GetDBServerIndex();
	if (index <= 0)
	{
		COUT_LOG(LOG_CERROR, "数据库链接失败");
		return;
	}
	TCPSocketInfo* tcpInfo = DTCPC->GetTCPSocketInfo(index);
	if (!tcpInfo)
	{
		COUT_LOG(LOG_CERROR, "数据库链接失败");
		return;
	}
	Netmsg msg;
	msg << BaseCfgMgr.GetServerId() 
		<< player->GetID() 
		<< loadMysql.sqlName
		<< loadMysql.uMainID 
		<< loadMysql.uAssistantID 
		<< loadMysql.uIdentification;

	DTCPC->SendMsg(index, msg.str().c_str(), msg.str().size(),
		MsgCmd::MsgCmd_DBServer, (int)DataBaseSysMsgCmd::cs_load_player, 
		0, tcpInfo->bev, (unsigned int)MsgCmd::MsgCmd_DBServer);
}
void MysqlClient::LoadLoginMysql(std::string& userid, SLoadMysql& loadMysql)
{
	if (userid.empty())
	{
		COUT_LOG(LOG_CINFO, "用户名为空");
		return;
	}
	int index = DTCPC->GetDBServerIndex();
	if (index <= 0)
	{
		COUT_LOG(LOG_CERROR, "数据库链接失败");
		return;
	}
	TCPSocketInfo* tcpInfo = DTCPC->GetTCPSocketInfo(index);
	if (!tcpInfo)
	{
		COUT_LOG(LOG_CERROR, "数据库链接失败");
		return;
	}
	Netmsg msg;
	msg << BaseCfgMgr.GetServerId()
		<< userid
		<< loadMysql.sqlName
		<< loadMysql.uMainID
		<< loadMysql.uAssistantID
		<< loadMysql.uIdentification;

	DTCPC->SendMsg(index, msg.str().c_str(), msg.str().size(),
		MsgCmd::MsgCmd_DBServer, (int)DataBaseSysMsgCmd::cs_load_login,
		0, tcpInfo->bev, (unsigned int)MsgCmd::MsgCmd_DBServer);
}
void MysqlClient::LoadGlobalMysql(SLoadMysql& loadMysql)
{
	int index = DTCPC->GetDBServerIndex();
	if (index <= 0)
	{
		COUT_LOG(LOG_CERROR, "数据库链接失败");
		return;
	}
	TCPSocketInfo* tcpInfo = DTCPC->GetTCPSocketInfo(index);
	if (!tcpInfo)
	{
		COUT_LOG(LOG_CERROR, "数据库链接失败");
		return;
	}
	Netmsg msg;
	msg << BaseCfgMgr.GetServerId()
		<< loadMysql.sqlName
		<< loadMysql.uMainID
		<< loadMysql.uAssistantID
		<< loadMysql.uIdentification;

	DTCPC->SendMsg(index, msg.str().c_str(), msg.str().size(),
		MsgCmd::MsgCmd_DBServer, (int)DataBaseSysMsgCmd::cs_load_global,
		0, tcpInfo->bev, (unsigned int)MsgCmd::MsgCmd_DBServer);
}