#include "../stdafx.h"

MysqlClient::MysqlClient()
{
}
MysqlClient::~MysqlClient()
{
}

SLoadMysql::SLoadMysql(std::string sqlname, MsgCmd mainID, 
	unsigned int assistantID, MsgCmd identification, int sid) :
	sqlName(sqlname),
	uMainID(static_cast<unsigned int>(mainID)),
	uAssistantID(assistantID),
	uIdentification(static_cast<unsigned int>(identification)),
	serverid(sid)
{
	if (sid == 0)
	{
		serverid = G_CfgMgr->GetCBaseCfgMgr().GetServerId();
	}
}

// 创建
void MysqlClient::CreateLoginMysql(std::string name, int cnt/* = 4096*/)
{
	int index = G_NetClient->GetDBServerIndex();
	if (index < 0)
	{
		Log(CERR, "数据库链接失败");
		return;
	}
	TCPSocketInfo* tcpInfo = G_NetClient->GetTCPSocketInfo(index);
	if (!tcpInfo)
	{
		Log(CERR, "数据库链接失败");
		return;
	}

	Netmsg msg;
	msg << name << cnt;

	G_NetClient->SendMsg(index, msg.str().c_str(), msg.str().size(),
		MsgCmd::MsgCmd_DBServer, (int)DataBaseSysMsgCmd::cs_create_login,
		0, tcpInfo->bev, (unsigned int)MsgCmd::MsgCmd_DBServer);
}
void MysqlClient::CreateGlobalMysql(std::string name, int cnt/* = 4096*/)
{
	int index = G_NetClient->GetDBServerIndex();
	if (index < 0)
	{
		Log(CERR, "数据库链接失败");
		return;
	}
	TCPSocketInfo* tcpInfo = G_NetClient->GetTCPSocketInfo(index);
	if (!tcpInfo)
	{
		Log(CERR, "数据库链接失败");
		return;
	}

	Netmsg msg;
	msg << name << cnt;

	G_NetClient->SendMsg(index, msg.str().c_str(), msg.str().size(),
		MsgCmd::MsgCmd_DBServer, (int)DataBaseSysMsgCmd::cs_create_global,
		0, tcpInfo->bev, (unsigned int)MsgCmd::MsgCmd_DBServer);
}
void MysqlClient::CreatePlayerMysql(std::string name, int cnt/* = 4096*/)
{
	int index = G_NetClient->GetDBServerIndex();
	if (index < 0)
	{
		Log(CERR, "数据库链接失败");
		return;
	}
	TCPSocketInfo* tcpInfo = G_NetClient->GetTCPSocketInfo(index);
	if (!tcpInfo)
	{
		Log(CERR, "数据库链接失败");
		return;
	}

	Netmsg msg;
	msg << name << cnt;

	G_NetClient->SendMsg(index, msg.str().c_str(), msg.str().size(),
		MsgCmd::MsgCmd_DBServer, (int)DataBaseSysMsgCmd::cs_create_player,
		0, tcpInfo->bev, (unsigned int)MsgCmd::MsgCmd_DBServer);
}

// 加载
void MysqlClient::LoadPlayerMysql(uint64_t userid, SLoadMysql& loadMysql)
{
	int index = G_NetClient->GetDBServerIndex();
	if (index < 0)
	{
		Log(CERR, "数据库链接失败");
		return;
	}
	TCPSocketInfo* tcpInfo = G_NetClient->GetTCPSocketInfo(index);
	if (!tcpInfo)
	{
		Log(CERR, "数据库链接失败");
		return;
	}
	Netmsg msg;
	msg << loadMysql.serverid
		<< userid
		<< loadMysql.sqlName
		<< loadMysql.uMainID 
		<< loadMysql.uAssistantID 
		<< loadMysql.uIdentification;

	G_NetClient->SendMsg(index, msg.str().c_str(), msg.str().size(),
		MsgCmd::MsgCmd_DBServer, (int)DataBaseSysMsgCmd::cs_load_player, 
		0, tcpInfo->bev, (unsigned int)MsgCmd::MsgCmd_DBServer);
}
void MysqlClient::LoadLoginMysql(std::string& userid, SLoadMysql loadMysql)
{
	if (userid.empty())
	{
		Log(CINF, "用户名为空");
		return;
	}
	int index = G_NetClient->GetDBServerIndex();
	if (index < 0)
	{
		Log(CERR, "数据库链接失败");
		return;
	}
	TCPSocketInfo* tcpInfo = G_NetClient->GetTCPSocketInfo(index);
	if (!tcpInfo)
	{
		Log(CERR, "数据库链接失败");
		return;
	}
	Netmsg msg;
	msg << loadMysql.serverid
		<< userid
		<< loadMysql.sqlName
		<< loadMysql.uMainID
		<< loadMysql.uAssistantID
		<< loadMysql.uIdentification;

	G_NetClient->SendMsg(index, msg.str().c_str(), msg.str().size(),
		MsgCmd::MsgCmd_DBServer, (int)DataBaseSysMsgCmd::cs_load_login,
		0, tcpInfo->bev, (unsigned int)MsgCmd::MsgCmd_DBServer);
}
void MysqlClient::LoadGlobalMysql(SLoadMysql& loadMysql)
{
	int index = G_NetClient->GetDBServerIndex();
	if (index < 0)
	{
		Log(CERR, "数据库链接失败");
		return;
	}
	TCPSocketInfo* tcpInfo = G_NetClient->GetTCPSocketInfo(index);
	if (!tcpInfo)
	{
		Log(CERR, "数据库链接失败");
		return;
	}
	Netmsg msg;
	msg << loadMysql.serverid
		<< loadMysql.sqlName
		<< loadMysql.uMainID
		<< loadMysql.uAssistantID
		<< loadMysql.uIdentification;

	G_NetClient->SendMsg(index, msg.str().c_str(), msg.str().size(),
		MsgCmd::MsgCmd_DBServer, (int)DataBaseSysMsgCmd::cs_load_global,
		0, tcpInfo->bev, (unsigned int)MsgCmd::MsgCmd_DBServer);
}

// 保存
void MysqlClient::SaveReplaceLoginMysql(std::string& userid, std::string sqlName, std::string&& data, int sid)
{
	int index = G_NetClient->GetDBServerIndex();
	if (index < 0)
	{
		Log(CERR, "数据库链接失败");
		return;
	}
	TCPSocketInfo* tcpInfo = G_NetClient->GetTCPSocketInfo(index);
	if (!tcpInfo)
	{
		Log(CERR, "数据库链接失败");
		return;
	}
	if (sid == 0)
	{
		sid = G_CfgMgr->GetCBaseCfgMgr().GetServerId();
	}
	Netmsg msg;
	msg << sid
		<< userid
		<< sqlName
		<< data;

	G_NetClient->SendMsg(index, msg.str().c_str(), msg.str().size(),
		MsgCmd::MsgCmd_DBServer, (int)DataBaseSysMsgCmd::cs_save_replace_login,
		0, tcpInfo->bev, (unsigned int)MsgCmd::MsgCmd_DBServer);
}
void MysqlClient::SaveReplacePlayerMysql(uint64_t userid, std::string sqlName, std::string&& data, int sid)
{
	int index = G_NetClient->GetDBServerIndex();
	if (index < 0)
	{
		Log(CERR, "数据库链接失败");
		return;
	}
	TCPSocketInfo* tcpInfo = G_NetClient->GetTCPSocketInfo(index);
	if (!tcpInfo)
	{
		Log(CERR, "数据库链接失败");
		return;
	}
	if (sid == 0)
	{
		sid = G_CfgMgr->GetCBaseCfgMgr().GetServerId();
	}
	Netmsg msg;
	msg << sid
		<< userid
		<< sqlName
		<< data;

	G_NetClient->SendMsg(index, msg.str().c_str(), msg.str().size(),
		MsgCmd::MsgCmd_DBServer, (int)DataBaseSysMsgCmd::cs_save_replace_player,
		0, tcpInfo->bev, (unsigned int)MsgCmd::MsgCmd_DBServer);
}
void MysqlClient::SaveReplaceGlobalMysql(std::string sqlName, std::string&& data, int sid)
{
	int index = G_NetClient->GetDBServerIndex();
	if (index < 0)
	{
		Log(CERR, "数据库链接失败");
		return;
	}
	TCPSocketInfo* tcpInfo = G_NetClient->GetTCPSocketInfo(index);
	if (!tcpInfo)
	{
		Log(CERR, "数据库链接失败");
		return;
	}
	if (sid == 0)
	{
		sid = G_CfgMgr->GetCBaseCfgMgr().GetServerId();
	}
	Netmsg msg;
	msg << sid
		<< sqlName
		<< data;

	G_NetClient->SendMsg(index, msg.str().c_str(), msg.str().size(),
		MsgCmd::MsgCmd_DBServer, (int)DataBaseSysMsgCmd::cs_save_replace_global,
		0, tcpInfo->bev, (unsigned int)MsgCmd::MsgCmd_DBServer);
}