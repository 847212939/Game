#include "../stdafx.h"

PlayerPrep::PlayerPrep()
{
}
PlayerPrep::~PlayerPrep()
{

}
char PlayerPrep::m_CreateGlobalTable[CREATE_TABLE_LEN] ="CREATE TABLE IF NOT EXISTS `%s` ("
"`serverid` int(11) NOT NULL,"
"`data` varchar(%d) COLLATE utf8_unicode_ci DEFAULT NULL,"
"PRIMARY KEY(`serverid`) USING BTREE"
") ENGINE = MyISAM DEFAULT CHARSET = utf8 COLLATE = utf8_unicode_ci ROW_FORMAT = DYNAMIC; ";

char PlayerPrep::m_CreateLoginTable[CREATE_TABLE_LEN] = "CREATE TABLE IF NOT EXISTS `%s` ("
"`serverid` int(11) NOT NULL,"
"`userid` varchar(255) COLLATE utf8_croatian_ci NOT NULL,"
"`data` varchar(%d) COLLATE utf8_croatian_ci DEFAULT NULL,"
"PRIMARY KEY(`serverid`,`userid`)"
") ENGINE = MyISAM DEFAULT CHARSET = utf8 COLLATE = utf8_croatian_ci;";

char PlayerPrep::m_CreatePlayerTable[CREATE_TABLE_LEN] = "CREATE TABLE IF NOT EXISTS `%s` ("
"`serverid` int(11) NOT NULL,"
"`userid` bigint(20) NOT NULL,"
"`data` varchar(%d) COLLATE utf8_croatian_ci DEFAULT NULL,"
"PRIMARY KEY(`serverid`,`userid`)"
") ENGINE = MyISAM DEFAULT CHARSET = utf8 COLLATE = utf8_croatian_ci;";

char PlayerPrep::createdatabase[CREATE_TABLE_LEN] = "CREATE DATABASE IF NOT EXISTS game "
"default charset utf8 COLLATE utf8_unicode_ci";

// ��ʼ��
void PlayerPrep::Init()
{
	if (!InitDB())
	{
		Log(CERR, "Failed to initialize database");
		return;
	}

	G_NetClient->GetSockeThreadVec().push_back(new std::thread(&PlayerPrep::HandlerExecuteSqlThread, this));
}
bool PlayerPrep::InitDB()
{
	const CDbCfg& dbCfg = G_CfgMgr->GetCBaseCfgMgr().GetDbCfg();

	// �������ݿ�
	m_CMysqlHelperSave.init(dbCfg.ip.c_str(), dbCfg.user.c_str(), dbCfg.passwd.c_str(), dbCfg.database.c_str(), "", dbCfg.port);
	try
	{
		std::lock_guard<std::mutex> guard(m_mutex);
		m_CMysqlHelperSave.connect();
	}
	catch (MysqlHelper_Exception& excep)
	{
		Log(CERR, "�������ݿ�ʧ��:%s", excep.errorInfo.c_str());
		return false;
	}

	// �������ݿ�
	m_CMysqlHelperLoad.init(dbCfg.ip.c_str(), dbCfg.user.c_str(), dbCfg.passwd.c_str(), dbCfg.database.c_str(), "", dbCfg.port);
	try
	{
		std::lock_guard<std::mutex> guard(m_mutex);
		m_CMysqlHelperLoad.connect();
	}
	catch (MysqlHelper_Exception& excep)
	{
		Log(CERR, "�������ݿ�ʧ��:%s", excep.errorInfo.c_str());
		return false;
	}

	return true;
}

// ������Ϣ
void PlayerPrep::MessageDispatch(PlayerInfo* playerInfo)
{
	if (!playerInfo)
	{
		Log(CERR, "!playerInfo");
		return;
	}
	if (!playerInfo->pMsg)
	{
		Log(CERR, "!playerInfo->pMsg || !playerInfo->pTcpSockInfo");
		return;
	}
	unsigned int uMainID = playerInfo->pMsg->netMessageHead.uMainID;
	if (uMainID >= (unsigned int)MsgCmd::MsgCmd_End || uMainID <= (unsigned int)MsgCmd::MsgCmd_Begin)
	{
		Log(CERR, "û���ҵ���Ϣ���� cmd = %d", uMainID);
		return;
	}
	MessageDispatch((MsgCmd)uMainID, playerInfo);
}
void PlayerPrep::MessageDispatch(MsgCmd cmd, PlayerInfo* playerInfo)
{
	if (!playerInfo)
	{
		Log(CERR, "playerInfo = null cmd = %d", (int)cmd);
		return;
	}
	SocketReadLine* pMsg = playerInfo->pMsg;
	if (!pMsg)
	{
		Log(CERR, "pMsg = null cmd = %d", (int)cmd);
		return;
	}
	if (MsgCmd::MsgCmd_DBServer == (MsgCmd)pMsg->netMessageHead.uIdentification)
	{
		CallBackFun(cmd, playerInfo);
	}
}

// ��ȡ���ݿ�
CMysqlHelper& PlayerPrep::GetSaveCMysqlHelper()
{
	return m_CMysqlHelperSave;
}
CMysqlHelper& PlayerPrep::GetLoadCMysqlHelper()
{
	return m_CMysqlHelperLoad;
}
std::condition_variable& PlayerPrep::GetConditionVariable()
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

	Log(CERR, "There is already a callback for this message. Please check the code cmd = %d", cmd);
}
bool PlayerPrep::CallBackFun(MsgCmd cmd, PlayerInfo* playerInfo)
{
	MapNetFun::iterator it = m_NetCBFunMap.find(cmd);
	if (it == m_NetCBFunMap.end())
	{
		Log(CERR, "No corresponding callback function found cmd = %d", cmd);
		return false;
	}

	it->second(playerInfo);
	return true;
}

void PlayerPrep::SaveInsertSQL(std::string sqlName, uint64_t userId, std::string& data, std::string keyName/* = "userid"*/, std::string dataName/* = "data"*/)
{
	std::ostringstream msg;
	msg << userId;

	MapRecordData mpColumns;

	mpColumns.insert(std::make_pair(keyName, std::make_pair(FT::DB_INT, msg.str())));
	mpColumns.insert(std::make_pair(dataName, std::make_pair(FT::DB_STR, data)));

	m_mutex.lock();
	std::string sSql = m_CMysqlHelperSave.buildInsertSQL(sqlName, mpColumns);
	m_sqlList.push_back(sSql);
	m_mutex.unlock();
	m_cond.notify_one();
}
void PlayerPrep::SaveUpdateSQL(std::string sqlName, uint64_t userId, std::string& data, const std::string& sCondition, std::string keyName/* = "userid"*/, std::string dataName/* = "data"*/)
{
	std::ostringstream msg;
	msg << userId;

	MapRecordData mpColumns;

	mpColumns.insert(std::make_pair(keyName, std::make_pair(FT::DB_INT, msg.str())));
	mpColumns.insert(std::make_pair(dataName, std::make_pair(FT::DB_STR, data)));

	m_mutex.lock();
	std::string sSql = m_CMysqlHelperSave.buildUpdateSQL(sqlName, mpColumns, sCondition);
	m_sqlList.push_back(sSql);
	m_mutex.unlock();
	m_cond.notify_one();
}
void PlayerPrep::SaveReplaceGlobalMysql(std::string& sqlName, int serverid, std::string& data,
	std::string serveridName/* = "serverid"*/, std::string useridName/* = "userid"*/, std::string dataName/* = "data"*/)
{
	std::ostringstream msgServerid;
	msgServerid << serverid;

	MapRecordData mpColumns;

	mpColumns.insert(std::make_pair(serveridName, std::make_pair(FT::DB_INT, msgServerid.str())));
	mpColumns.insert(std::make_pair(dataName, std::make_pair(FT::DB_STR, data)));

	m_mutex.lock();
	std::string sSql = m_CMysqlHelperSave.buildReplaceSQL(sqlName, mpColumns);
	m_sqlList.push_back(sSql);
	m_mutex.unlock();
	m_cond.notify_one();
}
void PlayerPrep::SaveReplacePlayerMysql(std::string& sqlName, int serverid, uint64_t userid, std::string& data,
	std::string serveridName/* = "serverid"*/, std::string useridName/* = "userid"*/, std::string dataName/* = "data"*/)
{
	std::ostringstream msgServerid;
	msgServerid << serverid;

	std::ostringstream msgUserid;
	msgUserid << userid;

	MapRecordData mpColumns;

	mpColumns.insert(std::make_pair(serveridName, std::make_pair(FT::DB_INT, msgServerid.str())));
	mpColumns.insert(std::make_pair(useridName, std::make_pair(FT::DB_INT, msgUserid.str())));
	mpColumns.insert(std::make_pair(dataName, std::make_pair(FT::DB_STR, data)));

	m_mutex.lock();
	std::string sSql = m_CMysqlHelperSave.buildReplaceSQL(sqlName, mpColumns);
	m_sqlList.push_back(sSql);
	m_mutex.unlock();
	m_cond.notify_one();
}
void PlayerPrep::SaveReplaceLoginMysql(std::string& sqlName, int serverid, std::string& userid, std::string data,
	std::string serveridName/* = "serverid"*/, std::string useridName/* = "userid"*/, std::string dataName/* = "data"*/)
{
	MapRecordData mpColumns;

	std::ostringstream msg;
	msg << serverid;

	mpColumns.insert(std::make_pair(serveridName, std::make_pair(FT::DB_INT, msg.str())));
	mpColumns.insert(std::make_pair(useridName, std::make_pair(FT::DB_STR, userid)));
	mpColumns.insert(std::make_pair(dataName, std::make_pair(FT::DB_STR, data)));

	m_mutex.lock();
	std::string sSql = m_CMysqlHelperSave.buildReplaceSQL(sqlName, mpColumns);
	m_sqlList.push_back(sSql);
	m_mutex.unlock();
	m_cond.notify_one();
}
void PlayerPrep::SaveDeleteSQL(std::string sqlName, const std::string& sCondition)
{
	std::ostringstream sSql;
	sSql << "delete from " << sqlName << " " << sCondition;
	
	m_mutex.lock();
	m_sqlList.push_back(sSql.str());
	m_mutex.unlock();
	m_cond.notify_one();
}
void PlayerPrep::LoadGlobalMysql(std::string sqlName, int serverid, std::string& outStr, std::string dataStr/* = "data"*/)
{
	char sql[1024] = "";
	sprintf(sql, "select * from %s where serverid=%d", sqlName.c_str(), serverid);

	CMysqlHelper::MysqlData data;
	try
	{
		std::lock_guard<std::mutex> guard(m_mutex);
		m_CMysqlHelperLoad.queryRecord(sql, data);
	}
	catch (MysqlHelper_Exception& excep)
	{
		Log(CERR, "�������ݿ�ʧ��:%s", excep.errorInfo.c_str());
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
void PlayerPrep::LoadLoginMysql(std::string sqlName, int serverid, std::string& userId, std::string& outStr, std::string dataStr/* = "data"*/)
{
	char sql[1024] = "";
	sprintf(sql, "select * from %s where serverid=%d and userid='%s'", sqlName.c_str(), serverid, userId.c_str());

	CMysqlHelper::MysqlData data;
	try
	{
		std::lock_guard<std::mutex> guard(m_mutex);
		m_CMysqlHelperLoad.queryRecord(sql, data);
	}
	catch (MysqlHelper_Exception& excep)
	{
		Log(CERR, "�������ݿ�ʧ��:%s", excep.errorInfo.c_str());
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
void PlayerPrep::LoadPlayerMysql(std::string sqlName, int serverid, uint64_t userId, std::string& outStr, std::string dataStr)
{
	char sql[1024] = "";
	sprintf(sql, "select * from %s where serverid=%d and userid=%lld", sqlName.c_str(), serverid, userId);

	CMysqlHelper::MysqlData queryData;
	try
	{
		std::lock_guard<std::mutex> guard(m_mutex);
		m_CMysqlHelperLoad.queryRecord(sql, queryData);
	}
	catch (MysqlHelper_Exception& excep)
	{
		Log(CERR, "�������ݿ�ʧ��:%s", excep.errorInfo.c_str());
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
		std::lock_guard<std::mutex> guard(m_mutex);
		m_CMysqlHelperLoad.queryRecord(sql, queryData);
	}
	catch (MysqlHelper_Exception& excep)
	{
		Log(CERR, "�������ݿ�ʧ��:%s", excep.errorInfo.c_str());
		return false;
	}

	return true;
}

// ���ݿ����
void PlayerPrep::CreateGlobalTable(std::string name, int cnt)
{
	char sql[CREATE_TABLE_LEN] = "";
	int len = sprintf_s(sql, CREATE_TABLE_LEN, m_CreateGlobalTable, name.c_str(), cnt);
	CreateTableSql(sql);
}
void PlayerPrep::CreatePlayerTable(std::string name, int cnt)
{
	char sql[CREATE_TABLE_LEN] = "";
	int len = sprintf_s(sql, CREATE_TABLE_LEN, m_CreatePlayerTable, name.c_str(), cnt);
	CreateTableSql(sql);
}
void PlayerPrep::CreateLoginTable(std::string name, int cnt)
{
	char sql[CREATE_TABLE_LEN] = "";
	int len = sprintf_s(sql, CREATE_TABLE_LEN, m_CreateLoginTable, name.c_str(), cnt);
	CreateTableSql(sql);
}
void PlayerPrep::CreateTableSql(const char* sql)
{
	m_mutex.lock();
	m_sqlList.push_back(sql);
	m_mutex.unlock();
	m_cond.notify_one();
}

void PlayerPrep::HandleEexcuteMysql(std::string& sql)
{
	try
	{
		std::lock_guard<std::mutex> guard(m_mutex);
		m_CMysqlHelperSave.execute(sql);
	}
	catch (MysqlHelper_Exception& excep)
	{
		Log(CERR, "ִ�����ݿ�ʧ��:%s", excep.errorInfo.c_str());
	}
}
void PlayerPrep::HandlerExecuteSqlThread()
{
	std::string str;
	bool& run = G_NetClient->GetRuninged();
	while (run)
	{
		std::unique_lock<std::mutex> uniqLock(m_mutex);
		m_cond.wait(uniqLock, [this] {return this->m_sqlList.size() > 0; });

		std::string str = m_sqlList.front();
		m_sqlList.pop_front();
		uniqLock.unlock();

		if (!str.empty())
		{
			HandleEexcuteMysql(str);
		}
	}
	Log(CINF, "save mysql thread end");
}
