#include "../stdafx.h"

MysqlClient::MysqlClient()
{
}
MysqlClient::~MysqlClient()
{
}

SLoadMysql::SLoadMysql(unsigned int index, std::string sqlname, MsgCmd mainID,
	unsigned int assistantID, MsgCmd identification, int sid) :
	uIndex(index),
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
	if (G_NetClient->GetServerType() == ServiceType::SERVICE_TYPE_CROSS)
	{
		return;
	}
	int index = G_NetClient->GetDBServerIndex();
	if (index < 0)
	{
		return;
	}
	TCPSocketInfo* tcpInfo = G_NetClient->GetTCPSocketInfo(index);
	if (!tcpInfo)
	{
		return;
	}

	Netmsg msg;
	msg << (int)G_NetClient->GetServerType()
		<< name 
		<< cnt;

	G_NetClient->SendMsg(index, msg.os_str().c_str(), msg.os_size(),
		MsgCmd::MsgCmd_DBServer, (int)DataBaseSysMsgCmd::cs_create_login,
		0, tcpInfo->bev, (unsigned int)MsgCmd::MsgCmd_DBServer);
}
void MysqlClient::CreateGlobalMysql(std::string name, int cnt/* = 4096*/)
{
	if (G_NetClient->GetServerType() == ServiceType::SERVICE_TYPE_CROSS)
	{
		return;
	}
	int index = G_NetClient->GetDBServerIndex();
	if (index < 0)
	{
		return;
	}
	TCPSocketInfo* tcpInfo = G_NetClient->GetTCPSocketInfo(index);
	if (!tcpInfo)
	{
		return;
	}

	Netmsg msg;
	msg << (int)G_NetClient->GetServerType()
		<< name 
		<< cnt;

	G_NetClient->SendMsg(index, msg.os_str().c_str(), msg.os_size(),
		MsgCmd::MsgCmd_DBServer, (int)DataBaseSysMsgCmd::cs_create_global,
		0, tcpInfo->bev, (unsigned int)MsgCmd::MsgCmd_DBServer);
}
void MysqlClient::CreatePlayerMysql(std::string name, int cnt/* = 4096*/)
{
	if (G_NetClient->GetServerType() == ServiceType::SERVICE_TYPE_CROSS)
	{
		return;
	}
	int index = G_NetClient->GetDBServerIndex();
	if (index < 0)
	{
		return;
	}
	TCPSocketInfo* tcpInfo = G_NetClient->GetTCPSocketInfo(index);
	if (!tcpInfo)
	{
		return;
	}

	Netmsg msg;
	msg << (int)G_NetClient->GetServerType()
		<< name 
		<< cnt;

	G_NetClient->SendMsg(index, msg.os_str().c_str(), msg.os_size(),
		MsgCmd::MsgCmd_DBServer, (int)DataBaseSysMsgCmd::cs_create_player,
		0, tcpInfo->bev, (unsigned int)MsgCmd::MsgCmd_DBServer);
}

// 加载
void MysqlClient::LoadPlayerMysql(uint64_t userid, SLoadMysql& loadMysql)
{
	int index = G_NetClient->GetDBServerIndex();
	if (index < 0)
	{
		return;
	}
	TCPSocketInfo* pDBTcpInfo = G_NetClient->GetTCPSocketInfo(index);
	TCPSocketInfo* pTcpInfo = G_NetClient->GetTCPSocketInfo(loadMysql.uIndex);
	if (!pDBTcpInfo || !pTcpInfo)
	{
		return;
	}
	if (pTcpInfo->isCross)
	{
		return;
	}
	Netmsg msg;

	// 1.要发送的协议
	msg << loadMysql.uMainID
		<< loadMysql.uAssistantID
		<< loadMysql.uIdentification;

	// 2.玩家id，本服索引
	msg << userid
		<< loadMysql.uIndex;

	// 3.服务器类型，服务器id
	msg << (int)G_NetClient->GetServerType()
		<< loadMysql.serverid;

	// 4.数据库名称
	msg << loadMysql.sqlName;

	G_NetClient->SendMsg(index, msg.os_str().c_str(), msg.os_size(),
		MsgCmd::MsgCmd_DBServer, (int)DataBaseSysMsgCmd::cs_load_player, 
		0, pDBTcpInfo->bev, (unsigned int)MsgCmd::MsgCmd_DBServer, userid);
}
void MysqlClient::LoadLoginMysql(std::string& userid, SLoadMysql loadMysql)
{
	if (G_NetClient->GetServerType() == 
		ServiceType::SERVICE_TYPE_CROSS)
	{
		return;
	}
	if (userid.empty())
	{
		return;
	}
	int index = G_NetClient->GetDBServerIndex();
	if (index < 0)
	{
		return;
	}
	TCPSocketInfo* pDBTcpInfo = G_NetClient->GetTCPSocketInfo(index);
	TCPSocketInfo* pTcpInfo = G_NetClient->GetTCPSocketInfo(loadMysql.uIndex);
	if (!pDBTcpInfo || !pTcpInfo)
	{
		return;
	}
	if (pTcpInfo->isCross)
	{
		return;
	}
	
	// 1.要发送的协议
	Netmsg msg;
	msg << loadMysql.uMainID
		<< loadMysql.uAssistantID
		<< loadMysql.uIdentification;

	// 2.玩家账号密码，本服索引
	msg << userid
		<< loadMysql.uIndex;
	
	// 3.服务器类型，服务器id
	msg << (int)G_NetClient->GetServerType()
		<< loadMysql.serverid;

	// 4.数据库名称
	msg << loadMysql.sqlName;

	G_NetClient->SendMsg(index, msg.os_str().c_str(), msg.os_size(),
		MsgCmd::MsgCmd_DBServer, (int)DataBaseSysMsgCmd::cs_load_login,
		0, pDBTcpInfo->bev, (unsigned int)MsgCmd::MsgCmd_DBServer);
}
void MysqlClient::LoadGlobalMysql(SLoadMysql& loadMysql)
{
	int index = G_NetClient->GetDBServerIndex();
	if (index < 0)
	{
		return;
	}
	TCPSocketInfo* pDBTcpInfo = G_NetClient->GetTCPSocketInfo(index);
	TCPSocketInfo* pTcpInfo = G_NetClient->GetTCPSocketInfo(loadMysql.uIndex);
	if (!pDBTcpInfo || !pTcpInfo)
	{
		return;
	}
	Netmsg msg;

	// 1.要发送的协议
	msg << loadMysql.uMainID
		<< loadMysql.uAssistantID
		<< loadMysql.uIdentification;

	// 2.服务器类型，服务器id
	msg << (int)G_NetClient->GetServerType()
		<< loadMysql.serverid;

	// 3.数据库名称
	msg << loadMysql.sqlName;

	// 4.本服索引
	msg << loadMysql.uIndex;

	G_NetClient->SendMsg(index, msg.os_str().c_str(), msg.os_size(),
		MsgCmd::MsgCmd_DBServer, (int)DataBaseSysMsgCmd::cs_load_global,
		0, pDBTcpInfo->bev, (unsigned int)MsgCmd::MsgCmd_DBServer);
}

// 保存
void MysqlClient::SaveReplaceLoginMysql(std::string& userid, std::string sqlName, std::string&& data, int sid)
{
	int index = G_NetClient->GetDBServerIndex();
	if (index < 0)
	{
		return;
	}
	TCPSocketInfo* tcpInfo = G_NetClient->GetTCPSocketInfo(index);
	if (!tcpInfo)
	{
		return;
	}
	if (sid == 0)
	{
		sid = G_CfgMgr->GetCBaseCfgMgr().GetServerId();
	}
	Netmsg msg;
	msg << (int)G_NetClient->GetServerType()
		<< sid
		<< userid
		<< sqlName
		<< data;

	G_NetClient->SendMsg(index, msg.os_str().c_str(), msg.os_size(),
		MsgCmd::MsgCmd_DBServer, (int)DataBaseSysMsgCmd::cs_save_replace_login,
		0, tcpInfo->bev, (unsigned int)MsgCmd::MsgCmd_DBServer);
}
void MysqlClient::SaveReplacePlayerMysql(uint64_t userid, std::string sqlName, std::string&& data, int sid)
{
	int index = G_NetClient->GetDBServerIndex();
	if (index < 0)
	{
		return;
	}
	TCPSocketInfo* tcpInfo = G_NetClient->GetTCPSocketInfo(index);
	if (!tcpInfo)
	{
		return;
	}
	if (sid == 0)
	{
		sid = G_CfgMgr->GetCBaseCfgMgr().GetServerId();
	}
	Netmsg msg;
	msg << (int)G_NetClient->GetServerType()
		<< sid
		<< userid
		<< sqlName
		<< data;

	G_NetClient->SendMsg(index, msg.os_str().c_str(), msg.os_size(),
		MsgCmd::MsgCmd_DBServer, (int)DataBaseSysMsgCmd::cs_save_replace_player,
		0, tcpInfo->bev, (unsigned int)MsgCmd::MsgCmd_DBServer);
}
void MysqlClient::SaveReplaceGlobalMysql(std::string sqlName, std::string&& data, int sid)
{
	int index = G_NetClient->GetDBServerIndex();
	if (index < 0)
	{
		return;
	}
	TCPSocketInfo* tcpInfo = G_NetClient->GetTCPSocketInfo(index);
	if (!tcpInfo)
	{
		return;
	}
	if (sid == 0)
	{
		sid = G_CfgMgr->GetCBaseCfgMgr().GetServerId();
	}
	Netmsg msg;
	msg << (int)G_NetClient->GetServerType()
		<< sid
		<< sqlName
		<< data;

	G_NetClient->SendMsg(index, msg.os_str().c_str(), msg.os_size(),
		MsgCmd::MsgCmd_DBServer, (int)DataBaseSysMsgCmd::cs_save_replace_global,
		0, tcpInfo->bev, (unsigned int)MsgCmd::MsgCmd_DBServer);
}