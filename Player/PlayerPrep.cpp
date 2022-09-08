#include "../Game/stdafx.h"

char PlayerPrep::createptable[CREATE_TABLE_LEN] = "CREATE TABLE IF NOT EXISTS `%s` ("
"`userid` varchar(255) COLLATE utf8_unicode_ci NOT NULL,"
"`data` varchar(2048) COLLATE utf8_unicode_ci DEFAULT NULL,"
"PRIMARY KEY(`userid`) USING BTREE"
") ENGINE = MyISAM DEFAULT CHARSET = utf8 COLLATE = utf8_unicode_ci ROW_FORMAT = DYNAMIC;";
char PlayerPrep::createpptable[CREATE_TABLE_LEN] = "CREATE TABLE IF NOT EXISTS `%s` ("
"`userid` bigint(20) NOT NULL,"
"`data` varchar(2048) COLLATE utf8_unicode_ci DEFAULT NULL,"
"PRIMARY KEY(`userid`) USING BTREE"
") ENGINE = MyISAM DEFAULT CHARSET = utf8 COLLATE = utf8_unicode_ci ROW_FORMAT = DYNAMIC; ";
char PlayerPrep::createdatabase[CREATE_TABLE_LEN] = "CREATE DATABASE IF NOT EXISTS game "
"default charset utf8 COLLATE utf8_unicode_ci";

PlayerPrep::PlayerPrep() : m_pServerTimer(new CServerTimer[BaseCfgMgr.GetTimerCnt()])
{
}

PlayerPrep::~PlayerPrep()
{

}

void PlayerPrep::Init()
{
	if (!InitDB())
	{
		COUT_LOG(LOG_CERROR, "Failed to initialize database");
		return;
	}

	DTCPC->GetSockeThreadVec().push_back(new std::thread(&PlayerPrep::HandlerExecuteSqlThread, this));

	int timerCnt = BaseCfgMgr.GetTimerCnt();

	for (int i = 0; i < timerCnt; i++)
	{
		m_pServerTimer[i].Start();
	}

	DSC->Init();
}

// 启动数据库
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
		// 没处理
	}
	else if (playerInfo->pMsg->socketType == SocketType::SOCKET_TYPE_TCP)
	{
		MessageDispatch((MsgCmd)uMainID, playerInfo);
	}
}

// 分发消息
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
		DSC->MessageDispatch(cmd, playerInfo);
	}
}

// 创建角色
void PlayerPrep::CreatePlayer(LoginData& loginData)
{
	DPCC->CreatePlayer(loginData);
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

CServerTimer* PlayerPrep::GetCServerTimer()
{
	return m_pServerTimer;
}

void PlayerPrep::AddNetCallback(MsgCmd cmd, std::function<void(PlayerInfo*)>&& fun)
{
	NetFunMap::iterator it = m_NetCBFunMap.find(cmd);
	if (it == m_NetCBFunMap.end())
	{
		m_NetCBFunMap.insert(std::make_pair(cmd, fun));
		return;
	}

	COUT_LOG(LOG_CERROR, "There is already a callback for this message. Please check the code cmd = %d", cmd);
}

bool PlayerPrep::CallBackFun(MsgCmd cmd, PlayerInfo* playerInfo)
{
	NetFunMap::iterator it = m_NetCBFunMap.find(cmd);
	if (it == m_NetCBFunMap.end())
	{
		COUT_LOG(LOG_CERROR, "No corresponding callback function found cmd = %d", cmd);
		return false;
	}

	it->second(playerInfo);
	return true;
}

bool PlayerPrep::CallBackFun(TimerCmd cmd)
{
	TimerFunMap::iterator it = m_TimerFunMap.find(cmd);
	if (it == m_TimerFunMap.end())
	{
		COUT_LOG(LOG_CERROR, "No corresponding callback function found cmd = %d", cmd);
		return false;
	}

	it->second();
	return true;
}

void PlayerPrep::DelTimerCallback(TimerCmd cmd)
{
	TimerFunMap::iterator it = m_TimerFunMap.find(cmd);
	if (it == m_TimerFunMap.end())
	{
		return;
	}

	m_TimerFunMap.erase(it);
}

void PlayerPrep::AddTimerCallback(TimerCmd cmd, std::function<void()>&& fun)
{
	TimerFunMap::iterator it = m_TimerFunMap.find(cmd);
	if (it == m_TimerFunMap.end())
	{
		m_TimerFunMap.insert(std::make_pair(cmd, fun));
		return;
	}

	COUT_LOG(LOG_CERROR, "There is already a callback for this message. Please check the code cmd = %d", cmd);
}

//设定定时器
bool PlayerPrep::SetTimer(TimerCmd uTimerID, UINT uElapse, BYTE timerType/* = SERVERTIMER_TYPE_PERISIST*/)
{
	if (!m_pServerTimer)
	{
		COUT_LOG(LOG_CERROR, "no timer run");
		return false;
	}

	int timerCnt = BaseCfgMgr.GetTimerCnt();

	if (timerCnt <= 0 || timerCnt > MAX_TIMER_THRED_NUMS)
	{
		COUT_LOG(LOG_CERROR, "timer error");
		return false;
	}

	m_pServerTimer[(int)uTimerID % timerCnt].SetTimer((unsigned int)uTimerID, uElapse, timerType);

	return true;
}

//清除定时器
bool PlayerPrep::KillTimer(TimerCmd uTimerID)
{
	if (!m_pServerTimer)
	{
		COUT_LOG(LOG_CERROR, "no timer run");
		return false;
	}

	int timerCnt = BaseCfgMgr.GetTimerCnt();

	if (timerCnt <= 0 || timerCnt > MAX_TIMER_THRED_NUMS)
	{
		COUT_LOG(LOG_CERROR, "timer error");
		return false;
	}

	m_pServerTimer[(int)uTimerID % timerCnt].KillTimer((unsigned int)uTimerID);

	return true;
}

void PlayerPrep::CreateTableS(std::string name)
{
	char sql[CREATE_TABLE_LEN] = "";

	int len = sprintf_s(sql, CREATE_TABLE_LEN, createptable, name.c_str());

	CreateTableSql(sql);
}

void PlayerPrep::CreateTableI(std::string name)
{
	char sql[CREATE_TABLE_LEN] = "";

	int len = sprintf_s(sql, CREATE_TABLE_LEN, createpptable, name.c_str());

	CreateTableSql(sql);
}

// create table
void PlayerPrep::CreateTableSql(const char* sql)
{
	m_cond.GetMutex().lock();
	m_sqlList.push_back(sql);
	m_cond.GetMutex().unlock();

	m_cond.NotifyOne();
}

// insert mysql
void PlayerPrep::SaveInsertSQL(std::string sqlName, uint64_t userId, std::string& data, std::string keyName/* = "userid"*/, std::string dataName/* = "data"*/)
{
	std::ostringstream os;
	os << userId;

	RecordDataMap mpColumns;

	mpColumns.insert(std::make_pair(keyName, std::make_pair(FT::DB_INT, os.str())));
	mpColumns.insert(std::make_pair(dataName, std::make_pair(FT::DB_STR, data)));

	std::string sSql = m_CMysqlHelperSave.buildInsertSQL(sqlName, mpColumns);

	m_cond.GetMutex().lock();
	m_sqlList.push_back(sSql);
	m_cond.GetMutex().unlock();

	m_cond.NotifyOne();
}

// update mysql
void PlayerPrep::SaveUpdateSQL(std::string sqlName, uint64_t userId, std::string& data, const std::string& sCondition, std::string keyName/* = "userid"*/, std::string dataName/* = "data"*/)
{
	std::ostringstream os;
	os << userId;

	RecordDataMap mpColumns;

	mpColumns.insert(std::make_pair(keyName, std::make_pair(FT::DB_INT, os.str())));
	mpColumns.insert(std::make_pair(dataName, std::make_pair(FT::DB_STR, data)));

	std::string sSql = m_CMysqlHelperSave.buildUpdateSQL(sqlName, mpColumns, sCondition);

	m_cond.GetMutex().lock();
	m_sqlList.push_back(sSql);
	m_cond.GetMutex().unlock();

	m_cond.NotifyOne();
}

// Replace mysql
void PlayerPrep::SaveReplaceSQL(std::string sqlName, uint64_t userId, std::string& data, std::string keyName, std::string dataName)
{
	std::ostringstream os;
	os << userId;

	RecordDataMap mpColumns;

	mpColumns.insert(std::make_pair(keyName, std::make_pair(FT::DB_INT, os.str())));
	mpColumns.insert(std::make_pair(dataName, std::make_pair(FT::DB_STR, data)));

	std::string sSql = m_CMysqlHelperSave.buildReplaceSQL(sqlName, mpColumns);

	m_cond.GetMutex().lock();
	m_sqlList.push_back(sSql);
	m_cond.GetMutex().unlock();

	m_cond.NotifyOne();
}

void PlayerPrep::SaveReplaceSQL(std::string sqlName, std::string& userId, std::string data, std::string keyName, std::string dataName)
{
	RecordDataMap mpColumns;

	mpColumns.insert(std::make_pair(keyName, std::make_pair(FT::DB_STR, userId)));
	mpColumns.insert(std::make_pair(dataName, std::make_pair(FT::DB_STR, data)));

	std::string sSql = m_CMysqlHelperSave.buildReplaceSQL(sqlName, mpColumns);

	m_cond.GetMutex().lock();
	m_sqlList.push_back(sSql);
	m_cond.GetMutex().unlock();

	m_cond.NotifyOne();
}

// delete mysql
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

	SqlKeyDataMap& dataMap = data[0];
	SqlKeyDataMap::iterator it = dataMap.find(dataStr);
	if (it == dataMap.end())
	{
		return;
	}

	outStr = it->second;
}

// 加载一条数据库
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

	SqlKeyDataMap& dataMap = queryData[0];
	SqlKeyDataMap::iterator it = dataMap.find(dataStr);
	if (it == dataMap.end())
	{
		return;
	}

	outStr = it->second;
}

// 加载多条数据库
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

bool PlayerPrep::SwapMysqlList(SqlList& LSqlList, SqlList& RSqlList, bool& run)
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
	SqlList sqlList;
	SqlList& mysqlList = m_sqlList;
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
