#define _CRT_SECURE_NO_WARNINGS
#include "../Game/stdafx.h"

PlayerPreproces::PlayerPreproces(TCPClient* pTCPClient) :
	m_pTCPClient(pTCPClient),
	m_scene(this)
{
	initCallBackFun();
	InitDB();
	Run();
}

PlayerPreproces::~PlayerPreproces()
{

}

// 初始化消息回调函数
void PlayerPreproces::initCallBackFun()
{
	// 登录注册回调函数
	AddCallBackFun(MsgCmd::MsgCmd_Login, std::move(std::bind(&PlayerPreproces::LoginInAccount, this, std::placeholders::_1)));
}

// 登录
void PlayerPreproces::LoginInAccount(PlayerInfo* pPlayerInfo)
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
		COUT_LOG(LOG_CERROR, "网络链接关闭");
		return;
	}
	unsigned int uAssistantID = pPlayerInfo->m_pMsg->netMessageHead.uAssistantID;
	std::string str = (char*)pPlayerInfo->m_pData;
	CIstringstream is(str);
	std::string id, pw;
	is >> id >> pw;
	switch ((PlayerPreprocesCmd)uAssistantID)
	{
	case PlayerPreprocesCmd::cs_register:
	{
		Register(id, pw, pPlayerInfo);
		break;
	}
	case PlayerPreprocesCmd::cs_login:
	{
		if (LoginIn(id, pw, pPlayerInfo))
		{
			CreatePlayr(pPlayerInfo);
		}
		break;
	}
	default:
		break;
	}
}

// 注册账号
bool PlayerPreproces::Register(std::string& id, std::string& passwaed, PlayerInfo* pPlayerInfo)
{
	if (id.empty() || passwaed.empty())
	{
		// 账号不存在
		return false;
	}

	std::string userId = Util::CreateUuid();

	m_accountMap.insert(std::make_pair(id, passwaed));
	m_AccountUserIDMap.insert(std::make_pair(id, userId));

	// 保存数据库
	SaveReplaceSQL("useraccount", id, passwaed);
	SaveReplaceSQL("userid", id, userId);
	return true;
}

// 检查账号是否存在
bool PlayerPreproces::LoginIn(std::string& id, std::string& passwaed, PlayerInfo* pPlayerInfo)
{
	if (id.empty() || passwaed.empty())
	{
		// 账号不存在
		return false;
	}
	AccountMap::const_iterator it = m_accountMap.find(id);
	if (it != m_accountMap.end())
	{
		// 内存查询
		if (it->second != passwaed)
		{
			// 密码错误
			// send
			return false;
		}
		else
		{
			AccountUserIDMap::const_iterator userIdIt = m_AccountUserIDMap.find(id);
			if (userIdIt != m_AccountUserIDMap.end())
			{
				pPlayerInfo->m_userId = userIdIt->second;
			}
			return true;
		}
	}
	else
	{
		// 数据库查询
		std::string pw = LoadUserAccount(id);
		if (pw.empty())
		{
			// 账号错误
			// send
			return false;
		}
		if (pw != passwaed)
		{
			// 密码错误
			// send
			return false;
		}
		else
		{
			std::string userId = LoadUserId(id);
			if (!userId.empty())
			{
				pPlayerInfo->m_userId = userId;
			}
			m_accountMap.insert(std::make_pair(id, pw));
			m_AccountUserIDMap.insert(std::make_pair(id, userId));
			return true;
		}
	}
}

// 加载玩家userid
std::string PlayerPreproces::LoadUserId(std::string& id)
{
	CMysqlHelper::MysqlData data;
	LoadOneSql(id, "userid", data);
	if (data.size() > 0)
	{
		SqlKeyDataMap& dataMap = data[0];
		SqlKeyDataMap::iterator it = dataMap.find("data");
		if (it != dataMap.end())
		{
			return it->second;
		}
	}
	return "";
}

// 加载玩家账号信息
std::string PlayerPreproces::LoadUserAccount(std::string& id)
{
	CMysqlHelper::MysqlData data;
	LoadOneSql(id, "useraccount", data);
	if (data.size() > 0)
	{
		SqlKeyDataMap& dataMap = data[0];
		SqlKeyDataMap::iterator it = dataMap.find("data");
		if (it != dataMap.end())
		{
			return it->second;
		}
	}
	return "";
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

	COUT_LOG(LOG_CINFO, "mysql init successed...");
	return true;
}

bool PlayerPreproces::Run()
{
	std::vector<std::thread*>& threadVec = m_pTCPClient->GetSockeThreadVec();
	threadVec.push_back(new std::thread(&PlayerPreproces::HandlerExecuteSqlThread, this));
	return true;
}

// 数据库执行
void PlayerPreproces::HandlerExecuteSqlThread()
{
	COUT_LOG(LOG_CINFO, "PlayerPreproces::HandlerDBSave thread begin...");
	std::this_thread::sleep_for(std::chrono::seconds(2));

	while (m_pTCPClient->GetRuninged())
	{
		//进入挂起状态
		std::unique_lock<std::mutex> uniqLock(m_cond.GetMutex());
		m_cond.Wait(uniqLock, [this] { if (this->m_sqlList.size() > 0) { return true; } return false; });

		if (this->m_sqlList.size() <= 0)
		{
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
	COUT_LOG(LOG_CINFO, "PlayerPreproces::HandlerDBSave thread end...");
}

// 处理消息
void PlayerPreproces::HandlerMessage(PlayerInfo* pPlayerInfo)
{
	if (!pPlayerInfo->m_pMsg || !pPlayerInfo->m_pTcpSockInfo)
	{
		COUT_LOG(LOG_CERROR, "!pPlayerInfo->pMsg || !pPlayerInfo->pTcpSockInfo");
		return;
	}
	if (!pPlayerInfo->m_pTcpSockInfo->isConnect)
	{
		COUT_LOG(LOG_CERROR, "网络链接关闭");
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

	}
	else if (pPlayerInfo->m_pMsg->socketType == SocketType::SOCKET_TYPE_TCP)
	{
		DispatchMessage((MsgCmd)uMainID, pPlayerInfo);
	}
}

// 分发消息
void PlayerPreproces::DispatchMessage(MsgCmd cmd, PlayerInfo* pPlayerInfo)
{
	if (cmd == MsgCmd::MsgCmd_Login)
	{
		CallBackFun(cmd, pPlayerInfo);
	}
	else
	{
		if (pPlayerInfo->m_loadDbed)
		{
			m_scene.DispatchMessage(cmd, pPlayerInfo);
		}
	}
}

// 创建角色
bool PlayerPreproces::CreatePlayr(PlayerInfo* pPlayerInfo)
{
	return m_scene.CreatePlayr(pPlayerInfo);
}

// 获取通知条件变量
ConditionVariable& PlayerPreproces::GetConditionVariable()
{
	return m_cond;
}

// 获取玩家账户信息
PlayerPreproces::AccountMap& PlayerPreproces::GetAccountMap()
{
	return m_accountMap;
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
Scene& PlayerPreproces::GetScene()
{
	return m_scene;
}

// 获取回调函数map
PlayerPreproces::CallBackFunMap& PlayerPreproces::GetCallBackFunMap()
{
	return m_CallBackFunMap;
}

// 加入回调函数
void PlayerPreproces::AddCallBackFun(MsgCmd cmd, std::function<void(PlayerInfo*)>&& fun)
{
	CallBackFunMap::iterator it = m_CallBackFunMap.find(cmd);
	if (it == m_CallBackFunMap.end())
	{
		m_CallBackFunMap.insert(std::make_pair(cmd, fun));
		return;
	}

	COUT_LOG(LOG_CINFO, "已经存在该消息的回调请检查代码 cmd = %d", cmd);
}

// 回调函数
bool PlayerPreproces::CallBackFun(MsgCmd cmd, PlayerInfo* pPlayerInfo)
{
	CallBackFunMap::iterator it = m_CallBackFunMap.find(cmd);
	if (it == m_CallBackFunMap.end())
	{
		COUT_LOG(LOG_CERROR, "没有查找到相应的回调函数 cmd = %d", cmd);
		return false;
	}

	it->second(pPlayerInfo);
	return true;
}

// insert mysql
void PlayerPreproces::SaveInsertSQL(std::string sqlName, std::string name, std::string data, std::string keyName/* = "userid"*/, std::string dataName/* = "data"*/)
{
	CMysqlHelper::RECORD_DATA mpColumns;

	mpColumns.insert(std::make_pair(keyName, std::make_pair(CMysqlHelper::FT::DB_STR, name)));
	mpColumns.insert(std::make_pair(dataName, std::make_pair(CMysqlHelper::FT::DB_STR, data)));

	std::string sSql = m_CMysqlHelper.buildInsertSQL(sqlName, mpColumns);

	m_cond.GetMutex().lock();
	m_sqlList.push_back(sSql);
	m_cond.GetMutex().unlock();

	m_cond.NotifyOne();
}

// update mysql
void PlayerPreproces::SaveUpdateSQL(std::string sqlName, std::string name, std::string data, const std::string& sCondition, std::string keyName/* = "userid"*/, std::string dataName/* = "data"*/)
{
	CMysqlHelper::RECORD_DATA mpColumns;

	mpColumns.insert(std::make_pair(keyName, std::make_pair(CMysqlHelper::FT::DB_STR, name)));
	mpColumns.insert(std::make_pair(dataName, std::make_pair(CMysqlHelper::FT::DB_STR, data)));

	std::string sSql = m_CMysqlHelper.buildUpdateSQL(sqlName, mpColumns, sCondition);

	m_cond.GetMutex().lock();
	m_sqlList.push_back(sSql);
	m_cond.GetMutex().unlock();

	m_cond.NotifyOne();
}

// Replace mysql
void PlayerPreproces::SaveReplaceSQL(std::string sqlName, std::string name, std::string data, std::string keyName/* = "userid"*/, std::string dataName/* = "data"*/)
{
	CMysqlHelper::RECORD_DATA mpColumns;

	mpColumns.insert(std::make_pair(keyName, std::make_pair(CMysqlHelper::FT::DB_STR, name)));
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

// 加载一条数据库
bool PlayerPreproces::LoadOneSql(std::string userId, std::string sqlName, CMysqlHelper::MysqlData& queryData, std::string dataStr /*= "data"*/)
{
	char sql[1024] = "";
	sprintf(sql, "select * from %s where userid=%s", sqlName.c_str(), userId.c_str());

	try
	{
		m_CMysqlHelper.queryRecord(sql, queryData);
	}
	catch (MysqlHelper_Exception& excep)
	{
		COUT_LOG(LOG_CERROR, "加载数据库失败:%s userId = %s", excep.errorInfo.c_str(), userId.c_str());
		return false;
	}

	return true;
}

// 加载多条数据库
bool PlayerPreproces::LoadMulitySql(std::string userId, std::string sqlName, CMysqlHelper::MysqlData& queryData, std::string dataStr /*= "data"*/)
{
	char sql[1024] = "";
	sprintf(sql, "select * from %s", sqlName.c_str());

	try
	{
		m_CMysqlHelper.queryRecord(sql, queryData);
	}
	catch (MysqlHelper_Exception& excep)
	{
		COUT_LOG(LOG_CERROR, "加载数据库失败:%s userId = %s", excep.errorInfo.c_str(), userId.c_str());
		return false;
	}

	return true;
}