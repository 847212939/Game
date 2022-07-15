#define _CRT_SECURE_NO_WARNINGS
#include "../Game/stdafx.h"

PlayerPreproces::PlayerPreproces(TCPClient* pTCPClient) :
	m_pTCPClient(pTCPClient)
{
}

PlayerPreproces::~PlayerPreproces()
{

}

void PlayerPreproces::Init()
{
	m_SubScene.SetSubPlayerPreproces(dynamic_cast<SubPlayerPreproces*>(this));

	InitDB();
	RunThread();
	m_SubScene.Init();
}

// 启动数据库
bool PlayerPreproces::InitDB()
{
	CBaseCfgMgr& baseCfgMgr = CfgMgr()->GetCBaseCfgMgr();
	const DbCfg& dbCfg = baseCfgMgr.GetDbCfg();

	// 链接数据库
	m_CMysqlHelper.init(dbCfg.ip.c_str(), dbCfg.user.c_str(), dbCfg.passwd.c_str(), dbCfg.database.c_str(), "", dbCfg.port);
	try
	{
		m_CMysqlHelper.connect();
	}
	catch (MysqlHelper_Exception& excep)
	{
		COUT_LOG(LOG_CERROR, "连接数据库失败:%s", excep.errorInfo.c_str());
		return false;
	}

	return true;
}

bool PlayerPreproces::RunThread()
{
	if (!m_pTCPClient)
	{
		COUT_LOG(LOG_CERROR, "TCP Client init fail");
	}

	m_pTCPClient->GetSockeThreadVec().push_back(new std::thread(&PlayerPreproces::HandlerExecuteSqlThread, this));

	return true;
}

// 数据库执行
void PlayerPreproces::HandlerExecuteSqlThread()
{
	if (!m_pTCPClient->GetRuninged())
	{
		COUT_LOG(LOG_CERROR, "PlayerPreproces::HandlerExecuteSqlThread 初始化未完成");
		return;
	}
	bool& run = m_pTCPClient->GetRuninged();

	while (run)
	{
		//进入挂起状态
		std::unique_lock<std::mutex> uniqLock(m_cond.GetMutex());
		m_cond.Wait(uniqLock, [this, &run] { if (this->m_sqlList.size() > 0 || !run) { return true; } return false; });
		
		if (this->m_sqlList.size() <= 0)
		{
			uniqLock.unlock();
			continue;
		}

		SqlList sqlList;
		std::swap(sqlList, m_sqlList);

		uniqLock.unlock();

		while (!sqlList.empty())
		{
			std::string sql = sqlList.front();
			sqlList.pop_front();

			try
			{
				m_CMysqlHelper.execute(sql);
			}
			catch (MysqlHelper_Exception& excep)
			{
				COUT_LOG(LOG_CERROR, "执行数据库失败:%s", excep.errorInfo.c_str());
			}
		}
	}
	COUT_LOG(LOG_CINFO, "save mysql thread end...");
}

// 处理消息
void PlayerPreproces::HandlerMessage(PlayerInfo* pPlayerInfo)
{
	if (!pPlayerInfo)
	{
		COUT_LOG(LOG_CERROR, "!pPlayerInfo");
		return;
	}
	if (!pPlayerInfo->m_pMsg || !pPlayerInfo->m_pTcpSockInfo)
	{
		COUT_LOG(LOG_CERROR, "!pPlayerInfo->pMsg || !pPlayerInfo->pTcpSockInfo");
		return;
	}
	if (!pPlayerInfo->m_pTcpSockInfo->isConnect)
	{
		COUT_LOG(LOG_CERROR, "Network link closed");
		return;
	}
	unsigned int uMainID = pPlayerInfo->m_pMsg->netMessageHead.uMainID;
	if (uMainID >= (unsigned int)MsgCmd::MsgCmd_End || uMainID <= (unsigned int)MsgCmd::MsgCmd_Begin)
	{
		COUT_LOG(LOG_CERROR, "没有找到消息类型 cmd = %d", uMainID);
		return;
	}
	// websocket服务器
	if (pPlayerInfo->m_pMsg->socketType == SocketType::SOCKET_TYPE_WEBSOCKET)
	{
		// 没处理
	}
	else if (pPlayerInfo->m_pMsg->socketType == SocketType::SOCKET_TYPE_TCP)
	{
		DispatchMessage((MsgCmd)uMainID, pPlayerInfo);
	}
}

// 分发消息
void PlayerPreproces::DispatchMessage(MsgCmd cmd, PlayerInfo* pPlayerInfo)
{
	switch (cmd)
	{
	case MsgCmd::MsgCmd_Login:
		// 处理登录协议等.. 玩家没有创建
		CallBackFun(cmd, pPlayerInfo);
		break;
	default:
		m_SubScene.DispatchMessage(cmd, pPlayerInfo);
		break;
	}
}

// 创建角色
void PlayerPreproces::CreatePlayer(unsigned int index, const TCPSocketInfo* pSockInfo, std::string& id, std::string& pw)
{
	m_SubScene.GetPlayerCenter().CreatePlayer(index, pSockInfo, id, pw);
}

// 获取通知条件变量
ConditionVariable& PlayerPreproces::GetConditionVariable()
{
	return m_cond;
}

// 获取网络句柄
TCPClient* PlayerPreproces::GetTCPClient()
{
	if (m_pTCPClient)
	{
		return m_pTCPClient;
	}

	return nullptr;
}

// 获取数据库
CMysqlHelper& PlayerPreproces::GetCMysqlHelper()
{
	return m_CMysqlHelper;
}

// 获取场景
SubScene& PlayerPreproces::GetSubScene()
{
	return m_SubScene;
}

// 加入回调函数
void PlayerPreproces::AddNetCallback(MsgCmd cmd, std::function<void(PlayerInfo*)>&& fun)
{
	NetFunMap::iterator it = m_NetCBFunMap.find(cmd);
	if (it == m_NetCBFunMap.end())
	{
		m_NetCBFunMap.insert(std::make_pair(cmd, fun));
		return;
	}

	COUT_LOG(LOG_CINFO, "There is already a callback for this message. Please check the code cmd = %d", cmd);
}

// 回调函数
bool PlayerPreproces::CallBackFun(MsgCmd cmd, PlayerInfo* pPlayerInfo)
{
	NetFunMap::iterator it = m_NetCBFunMap.find(cmd);
	if (it == m_NetCBFunMap.end())
	{
		COUT_LOG(LOG_CERROR, "No corresponding callback function found cmd = %d", cmd);
		return false;
	}

	it->second(pPlayerInfo);
	return true;
}

char PlayerPreproces::createptable[CreateTableLen] = "CREATE TABLE IF NOT EXISTS `%s` ("
"`userid` varchar(255) COLLATE utf8_unicode_ci NOT NULL,"
"`data` varchar(1024) COLLATE utf8_unicode_ci DEFAULT NULL,"
"PRIMARY KEY(`userid`) USING BTREE"
") ENGINE = MyISAM DEFAULT CHARSET = utf8 COLLATE = utf8_unicode_ci ROW_FORMAT = DYNAMIC;";
char PlayerPreproces::createpptable[CreateTableLen] = "CREATE TABLE IF NOT EXISTS `%s` ("
"`userid` bigint(20) NOT NULL,"
"`data` varchar(1024) COLLATE utf8_unicode_ci DEFAULT NULL,"
"PRIMARY KEY(`userid`) USING BTREE"
") ENGINE = MyISAM DEFAULT CHARSET = utf8 COLLATE = utf8_unicode_ci ROW_FORMAT = DYNAMIC; ";

void PlayerPreproces::CreateTableS(std::string name)
{
	char sql[CreateTableLen] = "";

	sprintf_s(sql, CreateTableLen, createptable, name.c_str());

	CreateTableSql(sql);
}

void PlayerPreproces::CreateTableI(std::string name)
{
	char sql[CreateTableLen] = "";

	sprintf_s(sql, CreateTableLen, createpptable, name.c_str());

	CreateTableSql(sql);
}

// create table
void PlayerPreproces::CreateTableSql(const char* sql)
{
	m_cond.GetMutex().lock();
	m_sqlList.push_back(sql);
	m_cond.GetMutex().unlock();

	m_cond.NotifyOne();
}

// insert mysql
void PlayerPreproces::SaveInsertSQL(std::string sqlName, uint64_t userId, std::string data, std::string keyName/* = "userid"*/, std::string dataName/* = "data"*/)
{
	std::ostringstream os;
	os << userId;

	CMysqlHelper::RECORD_DATA mpColumns;

	mpColumns.insert(std::make_pair(keyName, std::make_pair(CMysqlHelper::FT::DB_INT, os.str())));
	mpColumns.insert(std::make_pair(dataName, std::make_pair(CMysqlHelper::FT::DB_STR, data)));

	std::string sSql = m_CMysqlHelper.buildInsertSQL(sqlName, mpColumns);

	m_cond.GetMutex().lock();
	m_sqlList.push_back(sSql);
	m_cond.GetMutex().unlock();

	m_cond.NotifyOne();
}

// update mysql
void PlayerPreproces::SaveUpdateSQL(std::string sqlName, uint64_t userId, std::string data, const std::string& sCondition, std::string keyName/* = "userid"*/, std::string dataName/* = "data"*/)
{
	std::ostringstream os;
	os << userId;

	CMysqlHelper::RECORD_DATA mpColumns;

	mpColumns.insert(std::make_pair(keyName, std::make_pair(CMysqlHelper::FT::DB_INT, os.str())));
	mpColumns.insert(std::make_pair(dataName, std::make_pair(CMysqlHelper::FT::DB_STR, data)));

	std::string sSql = m_CMysqlHelper.buildUpdateSQL(sqlName, mpColumns, sCondition);

	m_cond.GetMutex().lock();
	m_sqlList.push_back(sSql);
	m_cond.GetMutex().unlock();

	m_cond.NotifyOne();
}

// Replace mysql
void PlayerPreproces::SaveReplaceSQL(std::string sqlName, uint64_t userId, std::string data, std::string keyName, std::string dataName)
{
	std::ostringstream os;
	os << userId;

	CMysqlHelper::RECORD_DATA mpColumns;
	
	mpColumns.insert(std::make_pair(keyName, std::make_pair(CMysqlHelper::FT::DB_INT, os.str())));
	mpColumns.insert(std::make_pair(dataName, std::make_pair(CMysqlHelper::FT::DB_STR, data)));

	std::string sSql = m_CMysqlHelper.buildReplaceSQL(sqlName, mpColumns);

	m_cond.GetMutex().lock();
	m_sqlList.push_back(sSql);
	m_cond.GetMutex().unlock();

	m_cond.NotifyOne();
}

void PlayerPreproces::SaveReplaceSQL(std::string sqlName, std::string userId, std::string data, std::string keyName, std::string dataName)
{
	CMysqlHelper::RECORD_DATA mpColumns;

	mpColumns.insert(std::make_pair(keyName, std::make_pair(CMysqlHelper::FT::DB_STR, userId)));
	mpColumns.insert(std::make_pair(dataName, std::make_pair(CMysqlHelper::FT::DB_STR, data)));

	std::string sSql = m_CMysqlHelper.buildReplaceSQL(sqlName, mpColumns);

	m_cond.GetMutex().lock();
	m_sqlList.push_back(sSql);
	m_cond.GetMutex().unlock();

	m_cond.NotifyOne();
}

// delete mysql
void PlayerPreproces::SaveDeleteSQL(std::string sqlName, const std::string& sCondition)
{
	std::ostringstream sSql;
	sSql << "delete from " << sqlName << " " << sCondition;

	m_cond.GetMutex().lock();
	m_sqlList.push_back(sSql.str());
	m_cond.GetMutex().unlock();

	m_cond.NotifyOne();
}

std::string PlayerPreproces::LoadOneSql(std::string userId, std::string sqlName, std::string dataStr /*= "data"*/)
{
	char sql[1024] = "";
	sprintf(sql, "select * from %s where userid=%s", sqlName.c_str(), userId.c_str());

	CMysqlHelper::MysqlData data;
	try
	{
		m_CMysqlHelper.queryRecord(sql, data);
	}
	catch (MysqlHelper_Exception& excep)
	{
		COUT_LOG(LOG_CERROR, "加载数据库失败:%s", excep.errorInfo.c_str());
		return "";
	}

	if (data.size() <= 0)
	{
		return "";
	}

	SqlKeyDataMap& dataMap = data[0];
	SqlKeyDataMap::iterator it = dataMap.find(dataStr);
	if (it == dataMap.end())
	{
		return "";
	}

	return it->second;
}

// 加载一条数据库
std::string PlayerPreproces::LoadOneSql(std::string sqlName, uint64_t userId, std::string dataStr)
{
	char sql[1024] = "";
	sprintf(sql, "select * from %s where userid=%lld", sqlName.c_str(), userId);

	CMysqlHelper::MysqlData queryData;
	try
	{
		m_CMysqlHelper.queryRecord(sql, queryData);
	}
	catch (MysqlHelper_Exception& excep)
	{
		COUT_LOG(LOG_CERROR, "加载数据库失败:%s", excep.errorInfo.c_str());
		return "";
	}
	if (queryData.size() <= 0)
	{
		return "";
	}

	SqlKeyDataMap& dataMap = queryData[0];
	SqlKeyDataMap::iterator it = dataMap.find(dataStr);
	if (it == dataMap.end())
	{
		return "";
	}

	return it->second;
}

// 加载多条数据库
bool PlayerPreproces::LoadMulitySql(std::string sqlName, uint64_t userId, CMysqlHelper::MysqlData& queryData, std::string dataStr)
{
	char sql[1024] = "";
	sprintf(sql, "select * from %s", sqlName.c_str());

	try
	{
		m_CMysqlHelper.queryRecord(sql, queryData);
	}
	catch (MysqlHelper_Exception& excep)
	{
		COUT_LOG(LOG_CERROR, "加载数据库失败:%s", excep.errorInfo.c_str());
		return false;
	}

	return true;
}