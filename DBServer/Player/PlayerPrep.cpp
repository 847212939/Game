#include "../stdafx.h"

char PlayerPrep::createptable[CREATE_TABLE_LEN] = "CREATE TABLE IF NOT EXISTS `%s` ("
"`userid` varchar(255) COLLATE utf8_unicode_ci NOT NULL,"
"`data` varchar(%d) COLLATE utf8_unicode_ci DEFAULT NULL,"
"PRIMARY KEY(`userid`) USING BTREE"
") ENGINE = MyISAM DEFAULT CHARSET = utf8 COLLATE = utf8_unicode_ci ROW_FORMAT = DYNAMIC;";
char PlayerPrep::createpptable[CREATE_TABLE_LEN] = "CREATE TABLE IF NOT EXISTS `%s` ("
"`userid` bigint(20) NOT NULL,"
"`data` varchar(%d) COLLATE utf8_unicode_ci DEFAULT NULL,"
"PRIMARY KEY(`userid`) USING BTREE"
") ENGINE = MyISAM DEFAULT CHARSET = utf8 COLLATE = utf8_unicode_ci ROW_FORMAT = DYNAMIC; ";
char PlayerPrep::createdatabase[CREATE_TABLE_LEN] = "CREATE DATABASE IF NOT EXISTS game "
"default charset utf8 COLLATE utf8_unicode_ci";

PlayerPrep::PlayerPrep()
{
}
PlayerPrep::~PlayerPrep()
{

}

// 初始化
void PlayerPrep::Init()
{
	if (!InitDB())
	{
		COUT_LOG(LOG_CERROR, "Failed to initialize database");
		return;
	}

	DTCPC->GetSockeThreadVec().push_back(new std::thread(&PlayerPrep::HandlerExecuteSqlThread, this));
}
bool PlayerPrep::InitDB()
{
	const CDbCfg& dbCfg = BaseCfgMgr.GetDbCfg();

	// 链接数据库
	m_CMysqlHelperSave.init(dbCfg.ip.c_str(), dbCfg.user.c_str(), dbCfg.passwd.c_str(), dbCfg.database.c_str(), "", dbCfg.port);
	try
	{
		m_CMysqlHelperSave.connect();
	}
	catch (MysqlHelper_Exception& excep)
	{
		COUT_LOG(LOG_CERROR, "连接数据库失败:%s", excep.errorInfo.c_str());
		return false;
	}

	// 链接数据库
	m_CMysqlHelperLoad.init(dbCfg.ip.c_str(), dbCfg.user.c_str(), dbCfg.passwd.c_str(), dbCfg.database.c_str(), "", dbCfg.port);
	try
	{
		m_CMysqlHelperLoad.connect();
	}
	catch (MysqlHelper_Exception& excep)
	{
		COUT_LOG(LOG_CERROR, "连接数据库失败:%s", excep.errorInfo.c_str());
		return false;
	}

	return true;
}

// 处理消息
void PlayerPrep::MessageDispatch(PlayerInfo* playerInfo)
{
	if (!playerInfo)
	{
		COUT_LOG(LOG_CERROR, "!playerInfo");
		return;
	}
	if (!playerInfo->pMsg)
	{
		COUT_LOG(LOG_CERROR, "!playerInfo->pMsg || !playerInfo->pTcpSockInfo");
		return;
	}
	auto* tcpInfo = DTCPC->GetTCPSocketInfo(playerInfo->pMsg->uIndex);
	if (!tcpInfo)
	{
		COUT_LOG(LOG_CERROR, "!tcpInfo");
		return;
	}
	if (tcpInfo->link != (uint64_t)MsgCmd::MsgCmd_Testlink)
	{
		DTCPC->CloseSocket(playerInfo->pMsg->uIndex);
		COUT_LOG(LOG_CERROR, "!tcpInfo->link != (uint64_t)MsgCmd::MsgCmd_Testlink");
		return;
	}
	unsigned int uMainID = playerInfo->pMsg->netMessageHead.uMainID;
	if (uMainID >= (unsigned int)MsgCmd::MsgCmd_End || uMainID <= (unsigned int)MsgCmd::MsgCmd_Begin)
	{
		COUT_LOG(LOG_CERROR, "没有找到消息类型 cmd = %d", uMainID);
		return;
	}
	// websocket服务器
	if (playerInfo->pMsg->socketType == SocketType::SOCKET_TYPE_WEBSOCKET)
	{
		MessageDispatch((MsgCmd)uMainID, playerInfo);
	}
	else if (playerInfo->pMsg->socketType == SocketType::SOCKET_TYPE_TCP)
	{
		MessageDispatch((MsgCmd)uMainID, playerInfo);
	}
}
void PlayerPrep::MessageDispatch(MsgCmd cmd, PlayerInfo* playerInfo)
{
	if (!playerInfo)
	{
		COUT_LOG(LOG_CERROR, "playerInfo = null cmd = %d", (int)cmd);
		return;
	}
	SocketReadLine* pMsg = playerInfo->pMsg;
	if (!pMsg)
	{
		COUT_LOG(LOG_CERROR, "pMsg = null cmd = %d", (int)cmd);
		return;
	}
	// 处理登录协议等.. 玩家没有创建
	if (MsgCmd::MsgCmd_PlayerPreproces == (MsgCmd)pMsg->netMessageHead.uIdentification)
	{
		CallBackFun(cmd, playerInfo);
	}
	else
	{
		
	}
}

// 获取数据库
CMysqlHelper& PlayerPrep::GetSaveCMysqlHelper()
{
	return m_CMysqlHelperSave;
}
CMysqlHelper& PlayerPrep::GetLoadCMysqlHelper()
{
	return m_CMysqlHelperLoad;
}
ConditionVariable& PlayerPrep::GetConditionVariable()
{
	return m_cond;
}

void PlayerPrep::AddNetCallback(MsgCmd cmd, std::function<void(PlayerInfo*)>&& fun)
{
	MapNetFun::iterator it = m_NetCBFunMap.find(cmd);
	if (it == m_NetCBFunMap.end())
	{
		m_NetCBFunMap.insert(std::make_pair(cmd, fun));
		return;
	}

	COUT_LOG(LOG_CERROR, "There is already a callback for this message. Please check the code cmd = %d", cmd);
}
bool PlayerPrep::CallBackFun(MsgCmd cmd, PlayerInfo* playerInfo)
{
	MapNetFun::iterator it = m_NetCBFunMap.find(cmd);
	if (it == m_NetCBFunMap.end())
	{
		COUT_LOG(LOG_CERROR, "No corresponding callback function found cmd = %d", cmd);
		return false;
	}

	it->second(playerInfo);
	return true;
}

// 数据库操作
void PlayerPrep::CreateTableS(std::string name, int cnt)
{
	char sql[CREATE_TABLE_LEN] = "";

	int len = sprintf_s(sql, CREATE_TABLE_LEN, createptable, name.c_str(), cnt);

	CreateTableSql(sql);
}
void PlayerPrep::CreateTableI(std::string name, int cnt)
{
	char sql[CREATE_TABLE_LEN] = "";

	int len = sprintf_s(sql, CREATE_TABLE_LEN, createpptable, name.c_str(), cnt);

	CreateTableSql(sql);
}
void PlayerPrep::CreateTableSql(const char* sql)
{
	m_cond.GetMutex().lock();
	m_sqlList.push_back(sql);
	m_cond.GetMutex().unlock();

	m_cond.NotifyOne();
}
void PlayerPrep::SaveInsertSQL(std::string sqlName, uint64_t userId, std::string& data, std::string keyName/* = "userid"*/, std::string dataName/* = "data"*/)
{
	std::ostringstream msg;
	msg << userId;

	MapRecordData mpColumns;

	mpColumns.insert(std::make_pair(keyName, std::make_pair(FT::DB_INT, msg.str())));
	mpColumns.insert(std::make_pair(dataName, std::make_pair(FT::DB_STR, data)));

	std::string sSql = m_CMysqlHelperSave.buildInsertSQL(sqlName, mpColumns);

	m_cond.GetMutex().lock();
	m_sqlList.push_back(sSql);
	m_cond.GetMutex().unlock();

	m_cond.NotifyOne();
}
void PlayerPrep::SaveUpdateSQL(std::string sqlName, uint64_t userId, std::string& data, const std::string& sCondition, std::string keyName/* = "userid"*/, std::string dataName/* = "data"*/)
{
	std::ostringstream msg;
	msg << userId;

	MapRecordData mpColumns;

	mpColumns.insert(std::make_pair(keyName, std::make_pair(FT::DB_INT, msg.str())));
	mpColumns.insert(std::make_pair(dataName, std::make_pair(FT::DB_STR, data)));

	std::string sSql = m_CMysqlHelperSave.buildUpdateSQL(sqlName, mpColumns, sCondition);

	m_cond.GetMutex().lock();
	m_sqlList.push_back(sSql);
	m_cond.GetMutex().unlock();

	m_cond.NotifyOne();
}
void PlayerPrep::SaveReplaceSQL(std::string sqlName, uint64_t userId, std::string& data, std::string keyName, std::string dataName)
{
	std::ostringstream msg;
	msg << userId;

	MapRecordData mpColumns;

	mpColumns.insert(std::make_pair(keyName, std::make_pair(FT::DB_INT, msg.str())));
	mpColumns.insert(std::make_pair(dataName, std::make_pair(FT::DB_STR, data)));

	std::string sSql = m_CMysqlHelperSave.buildReplaceSQL(sqlName, mpColumns);

	m_cond.GetMutex().lock();
	m_sqlList.push_back(sSql);
	m_cond.GetMutex().unlock();

	m_cond.NotifyOne();
}
void PlayerPrep::SaveReplaceSQL(std::string sqlName, std::string& userId, std::string data, std::string keyName, std::string dataName)
{
	MapRecordData mpColumns;

	mpColumns.insert(std::make_pair(keyName, std::make_pair(FT::DB_STR, userId)));
	mpColumns.insert(std::make_pair(dataName, std::make_pair(FT::DB_STR, data)));

	std::string sSql = m_CMysqlHelperSave.buildReplaceSQL(sqlName, mpColumns);

	m_cond.GetMutex().lock();
	m_sqlList.push_back(sSql);
	m_cond.GetMutex().unlock();

	m_cond.NotifyOne();
}
void PlayerPrep::SaveDeleteSQL(std::string sqlName, const std::string& sCondition)
{
	std::ostringstream sSql;
	sSql << "delete from " << sqlName << " " << sCondition;

	m_cond.GetMutex().lock();
	m_sqlList.push_back(sSql.str());
	m_cond.GetMutex().unlock();

	m_cond.NotifyOne();
}
void PlayerPrep::LoadOneSql(std::string& userId, std::string sqlName, std::string& outStr, std::string dataStr /*= "data"*/)
{
	char sql[1024] = "";
	sprintf(sql, "select * from %s where userid='%s'", sqlName.c_str(), userId.c_str());

	CMysqlHelper::MysqlData data;
	try
	{
		m_cond.GetMutex().lock();
		m_CMysqlHelperLoad.queryRecord(sql, data);
		m_cond.GetMutex().unlock();
	}
	catch (MysqlHelper_Exception& excep)
	{
		COUT_LOG(LOG_CERROR, "加载数据库失败:%s", excep.errorInfo.c_str());
		return;
	}

	if (data.size() <= 0)
	{
		return;
	}

	MapStringString& dataMap = data[0];
	MapStringString::iterator it = dataMap.find(dataStr);
	if (it == dataMap.end())
	{
		return;
	}

	outStr = it->second;
}
void PlayerPrep::LoadOneSql(std::string sqlName, uint64_t userId, std::string& outStr, std::string dataStr)
{
	char sql[1024] = "";
	sprintf(sql, "select * from %s where userid=%lld", sqlName.c_str(), userId);

	CMysqlHelper::MysqlData queryData;
	try
	{
		m_cond.GetMutex().lock();
		m_CMysqlHelperLoad.queryRecord(sql, queryData);
		m_cond.GetMutex().unlock();
	}
	catch (MysqlHelper_Exception& excep)
	{
		COUT_LOG(LOG_CERROR, "加载数据库失败:%s", excep.errorInfo.c_str());
		return;
	}
	if (queryData.size() <= 0)
	{
		return;
	}

	MapStringString& dataMap = queryData[0];
	MapStringString::iterator it = dataMap.find(dataStr);
	if (it == dataMap.end())
	{
		return;
	}

	outStr = it->second;
}
bool PlayerPrep::LoadMulitySql(std::string sqlName, uint64_t userId, CMysqlHelper::MysqlData& queryData, std::string dataStr)
{
	char sql[1024] = "";
	sprintf(sql, "select * from %s", sqlName.c_str());

	try
	{
		m_cond.GetMutex().lock();
		m_CMysqlHelperLoad.queryRecord(sql, queryData);
		m_cond.GetMutex().unlock();
	}
	catch (MysqlHelper_Exception& excep)
	{
		COUT_LOG(LOG_CERROR, "加载数据库失败:%s", excep.errorInfo.c_str());
		return false;
	}

	return true;
}

// 多线程下数据执行
bool PlayerPrep::SwapMysqlList(ListString& LSqlList, ListString& RSqlList, bool& run)
{
	RSqlList.clear();

	std::unique_lock<std::mutex> uniqLock(m_cond.GetMutex());
	m_cond.Wait(uniqLock, [&LSqlList, &run] 
	{ 
		if (LSqlList.size() > 0 || !run) 
		{ 
			return true; 
		} 
		return false; 
	});
	if (LSqlList.size() <= 0)
	{
		uniqLock.unlock();
		return false;
	}

	RSqlList.swap(LSqlList);

	uniqLock.unlock();

	return true;
}
void PlayerPrep::HandleEexcuteMysql(std::string& sql)
{
	try
	{
		m_CMysqlHelperSave.execute(sql);
	}
	catch (MysqlHelper_Exception& excep)
	{
		COUT_LOG(LOG_CERROR, "执行数据库失败:%s", excep.errorInfo.c_str());
	}
}
void PlayerPrep::HandlerExecuteSqlThread()
{
	ListString sqlList;
	ListString& mysqlList = m_sqlList;
	bool& run = DTCPC->GetRuninged();

	while (run)
	{
		if (!SwapMysqlList(mysqlList, sqlList, run))
		{
			continue;
		}
		while (!sqlList.empty())
		{
			HandleEexcuteMysql(sqlList.front());
			sqlList.pop_front();
		}
	}
	COUT_LOG(LOG_CINFO, "save mysql thread end");
}
