#include "../stdafx.h"

MysqlClient::MysqlClient()
{
}
MysqlClient::~MysqlClient()
{
}

// ����
void MysqlClient::CreateLoginMysql(std::string name, int cnt = 4096)
{
	int index = DTCPC->GetDBServerIndex();
	if (index <= 0)
	{
		COUT_LOG(LOG_CERROR, "���ݿ�����ʧ��");
		return;
	}
	TCPSocketInfo* tcpInfo = DTCPC->GetTCPSocketInfo(index);
	if (!tcpInfo)
	{
		COUT_LOG(LOG_CERROR, "���ݿ�����ʧ��");
		return;
	}

	Netmsg msg;
	msg << name << cnt;

	DTCPC->SendMsg(index, msg.str().c_str(), msg.str().size(),
		MsgCmd::MsgCmd_DBServer, (int)DataBaseSysMsgCmd::cs_create_login,
		0, tcpInfo->bev, (unsigned int)MsgCmd::MsgCmd_DBServer);
}
void MysqlClient::CreateGlobalMysql(std::string name, int cnt = 4096)
{
	int index = DTCPC->GetDBServerIndex();
	if (index <= 0)
	{
		COUT_LOG(LOG_CERROR, "���ݿ�����ʧ��");
		return;
	}
	TCPSocketInfo* tcpInfo = DTCPC->GetTCPSocketInfo(index);
	if (!tcpInfo)
	{
		COUT_LOG(LOG_CERROR, "���ݿ�����ʧ��");
		return;
	}

	Netmsg msg;
	msg << name << cnt;

	DTCPC->SendMsg(index, msg.str().c_str(), msg.str().size(),
		MsgCmd::MsgCmd_DBServer, (int)DataBaseSysMsgCmd::cs_create_global,
		0, tcpInfo->bev, (unsigned int)MsgCmd::MsgCmd_DBServer);
}
void MysqlClient::CreatePlayerMysql(std::string name, int cnt = 4096)
{
	int index = DTCPC->GetDBServerIndex();
	if (index <= 0)
	{
		COUT_LOG(LOG_CERROR, "���ݿ�����ʧ��");
		return;
	}
	TCPSocketInfo* tcpInfo = DTCPC->GetTCPSocketInfo(index);
	if (!tcpInfo)
	{
		COUT_LOG(LOG_CERROR, "���ݿ�����ʧ��");
		return;
	}

	Netmsg msg;
	msg << name << cnt;

	DTCPC->SendMsg(index, msg.str().c_str(), msg.str().size(),
		MsgCmd::MsgCmd_DBServer, (int)DataBaseSysMsgCmd::cs_create_player,
		0, tcpInfo->bev, (unsigned int)MsgCmd::MsgCmd_DBServer);
}

// ����
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
		COUT_LOG(LOG_CERROR, "���ݿ�����ʧ��");
		return;
	}
	TCPSocketInfo* tcpInfo = DTCPC->GetTCPSocketInfo(index);
	if (!tcpInfo)
	{
		COUT_LOG(LOG_CERROR, "���ݿ�����ʧ��");
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
void MysqlClient::LoadLoginMysql(std::string& userid, SLoadMysql loadMysql)
{
	if (userid.empty())
	{
		COUT_LOG(LOG_CINFO, "�û���Ϊ��");
		return;
	}
	int index = DTCPC->GetDBServerIndex();
	if (index <= 0)
	{
		COUT_LOG(LOG_CERROR, "���ݿ�����ʧ��");
		return;
	}
	TCPSocketInfo* tcpInfo = DTCPC->GetTCPSocketInfo(index);
	if (!tcpInfo)
	{
		COUT_LOG(LOG_CERROR, "���ݿ�����ʧ��");
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
		COUT_LOG(LOG_CERROR, "���ݿ�����ʧ��");
		return;
	}
	TCPSocketInfo* tcpInfo = DTCPC->GetTCPSocketInfo(index);
	if (!tcpInfo)
	{
		COUT_LOG(LOG_CERROR, "���ݿ�����ʧ��");
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

// ����
void MysqlClient::SaveReplacePlayerMysql(Player* player, std::string sqlName, std::string&& data)
{
	if (!player)
	{
		COUT_LOG(LOG_CERROR, "player null");
		return;
	}
	int index = DTCPC->GetDBServerIndex();
	if (index <= 0)
	{
		COUT_LOG(LOG_CERROR, "���ݿ�����ʧ��");
		return;
	}
	TCPSocketInfo* tcpInfo = DTCPC->GetTCPSocketInfo(index);
	if (!tcpInfo)
	{
		COUT_LOG(LOG_CERROR, "���ݿ�����ʧ��");
		return;
	}
	Netmsg msg;
	msg << BaseCfgMgr.GetServerId()
		<< player->GetID()
		<< sqlName
		<< data;

	DTCPC->SendMsg(index, msg.str().c_str(), msg.str().size(),
		MsgCmd::MsgCmd_DBServer, (int)DataBaseSysMsgCmd::cs_save_replace_player,
		0, tcpInfo->bev, (unsigned int)MsgCmd::MsgCmd_DBServer);
}
void MysqlClient::SaveReplaceGlobalMysql(std::string sqlName, std::string data)
{
	int index = DTCPC->GetDBServerIndex();
	if (index <= 0)
	{
		COUT_LOG(LOG_CERROR, "���ݿ�����ʧ��");
		return;
	}
	TCPSocketInfo* tcpInfo = DTCPC->GetTCPSocketInfo(index);
	if (!tcpInfo)
	{
		COUT_LOG(LOG_CERROR, "���ݿ�����ʧ��");
		return;
	}
	Netmsg msg;
	msg << BaseCfgMgr.GetServerId()
		<< sqlName
		<< data;

	DTCPC->SendMsg(index, msg.str().c_str(), msg.str().size(),
		MsgCmd::MsgCmd_DBServer, (int)DataBaseSysMsgCmd::cs_save_replace_global,
		0, tcpInfo->bev, (unsigned int)MsgCmd::MsgCmd_DBServer);
}