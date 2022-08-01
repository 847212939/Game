#define _CRT_SECURE_NO_WARNINGS
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

PlayerPrep::PlayerPrep(TCPClient* pTCPClient) :
	m_pTCPClient(pTCPClient),
	m_pServerTimer(new CServerTimer[BaseCfgMgr().GetTimerCnt()]),
	m_SqlPre("")
{
}

PlayerPrep::~PlayerPrep()
{

}

void PlayerPrep::Init()
{
	if (!m_pTCPClient)
	{
		COUT_LOG(LOG_CERROR, "m_pTCPClient = null");
		return;
	}
	if (!InitDB())
	{
		COUT_LOG(LOG_CERROR, "Failed to initialize database");
		return;
	}

	m_SceneClient.SetPlayerPrepClient(dynamic_cast<PlayerPrepClient*>(this));
	m_pTCPClient->GetSockeThreadVec().push_back(new std::thread(&PlayerPrep::HandlerExecuteSqlThread, this));

	int timerCnt = BaseCfgMgr().GetTimerCnt();

	for (int i = 0; i < timerCnt; i++)
	{
		m_pServerTimer[i].SetTCPClient(m_pTCPClient);
		m_pServerTimer[i].Start();
	}

	m_SceneClient.Init();
}

// 启动数据库
bool PlayerPrep::InitDB()
{
	const DbCfg& dbCfg = BaseCfgMgr().GetDbCfg();

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
void PlayerPrep::HandlerMessage(PlayerInfo* playerInfo)
{
	if (!playerInfo)
	{
		COUT_LOG(LOG_CERROR, "!playerInfo");
		return;
	}
	if (!playerInfo->m_pMsg || !playerInfo->m_pTcpSockInfo)
	{
		COUT_LOG(LOG_CERROR, "!playerInfo->pMsg || !playerInfo->pTcpSockInfo");
		return;
	}
	if (!playerInfo->m_pTcpSockInfo->isConnect)
	{
		COUT_LOG(LOG_CERROR, "Network link closed");
		return;
	}
	unsigned int uMainID = playerInfo->m_pMsg->netMessageHead.uMainID;
	if (uMainID >= (unsigned int)MsgCmd::MsgCmd_End || uMainID <= (unsigned int)MsgCmd::MsgCmd_Begin)
	{
		COUT_LOG(LOG_CERROR, "没有找到消息类型 cmd = %d", uMainID);
		return;
	}
	// websocket服务器
	if (playerInfo->m_pMsg->socketType == SocketType::SOCKET_TYPE_WEBSOCKET)
	{
		// 没处理
	}
	else if (playerInfo->m_pMsg->socketType == SocketType::SOCKET_TYPE_TCP)
	{
		MessageDispatch((MsgCmd)uMainID, playerInfo);
	}
}

// 分发消息
void PlayerPrep::MessageDispatch(MsgCmd cmd, PlayerInfo* playerInfo)
{
	// 处理登录协议等.. 玩家没有创建
	if (MsgCmd::MsgCmd_PlayerPreproces == cmd)
	{
		if (!playerInfo)
		{
			COUT_LOG(LOG_CERROR, "playerInfo = null cmd = %d", (int)cmd);
			return;
		}
		SocketReadLine* pMsg = playerInfo->m_pMsg;
		if (!pMsg)
		{
			COUT_LOG(LOG_CERROR, "pMsg = null cmd = %d", (int)cmd);
			return;
		}
		CallBackFun((MsgCmd)pMsg->netMessageHead.uAssistantID, playerInfo);
	}
	else
	{
		m_SceneClient.MessageDispatch(cmd, playerInfo);
	}
}

// 创建角色
void PlayerPrep::CreatePlayer(unsigned int index, const TCPSocketInfo* pSockInfo, std::string& id, std::string& pw)
{
	m_SceneClient.GetPlayerCenterClient().CreatePlayer(index, pSockInfo, id, pw);
}

// 获取网络句柄
TCPClient* PlayerPrep::GetTCPClient()
{
	if (m_pTCPClient)
	{
		return m_pTCPClient;
	}

	return nullptr;
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

// 获取场景
SceneClient& PlayerPrep::GetSceneClient()
{
	return m_SceneClient;
}

void PlayerPrep::AddNetCallback(MsgCmd cmd, std::function<void(PlayerInfo*)>&& fun)
{
	NetFunMap::iterator it = m_NetCBFunMap.find(cmd);
	if (it == m_NetCBFunMap.end())
	{
		m_NetCBFunMap.insert(std::make_pair(cmd, fun));
		return;
	}

	COUT_LOG(LOG_CINFO, "There is already a callback for this message. Please check the code cmd = %d", cmd);
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

	COUT_LOG(LOG_CINFO, "There is already a callback for this message. Please check the code cmd = %d", cmd);
}

//设定定时器
bool PlayerPrep::SetTimer(TimerCmd uTimerID, UINT uElapse, BYTE timerType/* = SERVERTIMER_TYPE_PERISIST*/)
{
	if (!m_pServerTimer)
	{
		COUT_LOG(LOG_CERROR, "no timer run");
		return false;
	}

	int timerCnt = BaseCfgMgr().GetTimerCnt();

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

	int timerCnt = BaseCfgMgr().GetTimerCnt();

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
	sprintf(sql, "select * from %s where userid=%s", sqlName.c_str(), userId.c_str());

	CMysqlHelper::MysqlData data;
	try
	{
		m_CMysqlHelperLoad.queryRecord(sql, data);
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
		m_CMysqlHelperLoad.queryRecord(sql, queryData);
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
		m_CMysqlHelperLoad.queryRecord(sql, queryData);
	}
	catch (MysqlHelper_Exception& excep)
	{
		COUT_LOG(LOG_CERROR, "加载数据库失败:%s", excep.errorInfo.c_str());
		return false;
	}

	return true;
}

// 数据库执行
void PlayerPrep::HandlerExecuteSqlThread()
{
	if (!m_pTCPClient)
	{
		COUT_LOG(LOG_CERROR, "initialization not complete");
		return;
	}
	if (!m_pTCPClient->GetRuninged())
	{
		COUT_LOG(LOG_CERROR, "PlayerPrep::HandlerExecuteSqlThread 初始化未完成");
		return;
	}

	SqlList& mysqlList = m_sqlList;
	bool& run = m_pTCPClient->GetRuninged();

	while (run)
	{
		std::unique_lock<std::mutex> uniqLock(m_cond.GetMutex());
		m_cond.Wait(uniqLock, [&mysqlList, &run] { if (mysqlList.size() > 0 || !run) { return true; } return false; });

		if (mysqlList.size() <= 0)
		{
			uniqLock.unlock();
			continue;
		}

		SqlList sqlList;
		sqlList.swap(mysqlList);

		uniqLock.unlock();

		while (!sqlList.empty())
		{
			std::string sql = sqlList.front();
			sqlList.pop_front();

			try
			{
				m_CMysqlHelperSave.execute(sql);
			}
			catch (MysqlHelper_Exception& excep)
			{
				COUT_LOG(LOG_CERROR, "执行数据库失败:%s", excep.errorInfo.c_str());
				if (m_SqlPre == sql)
				{
					continue;
				}
				m_cond.GetMutex().lock();
				mysqlList.push_front(sql);
				m_cond.GetMutex().unlock();

				m_SqlPre = sql;
				std::this_thread::sleep_for(std::chrono::milliseconds(50));
			}
		}
	}
	COUT_LOG(LOG_CINFO, "save mysql thread end...");
}
