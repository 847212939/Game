#include "../Game/stdafx.h"

PlayerPreproces::PlayerPreproces(TCPClient* pTCPClient) : 
	m_pTCPClient(pTCPClient), 
	m_PlayerCenter(this)
{
	intCallBackFun();
	InitDB();
	Run();
}

PlayerPreproces::~PlayerPreproces()
{

}

// 初始化消息回调函数
bool PlayerPreproces::intCallBackFun()
{
	// 登录注册回调函数
	AddCallBackFun(MsgCmd::MsgCmd_RegisterAccount, std::move(std::bind(&PlayerPreproces::RegisterAccount, this, std::placeholders::_1)));
	return true;
}

// 启动数据库
bool PlayerPreproces::InitDB()
{
	// 链接数据库
	m_CMysqlHelper.init("127.0.0.1", "root", "", "game", "", 3366);
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

// 注册账号
void PlayerPreproces::RegisterAccount(PlayerInfo* pPlayerInfo)
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
	switch ((PlayerPreprocesCmd)uAssistantID)
	{
	case PlayerPreprocesCmd::cs_register:
		break;
	case PlayerPreprocesCmd::cs_login:
		break;
	default:
		break;
	}
}

// 检查账号信息
bool PlayerPreproces::CheckUserAccount(std::string& id, std::string& passwaed)
{
	if (id.empty() || passwaed.empty())
	{
		// 账号或者密码错误
		// send
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
			// 账号密码验证通过
			// send
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
			m_accountMap[id] = pw;
			return false;
		}
		else
		{
			// 账号密码验证通过
			// send
			m_accountMap[id] = pw;
			return true;
		}
	}
}

// 加载玩家账号信息
std::string PlayerPreproces::LoadUserAccount(std::string& id)
{
	return "";
}

bool PlayerPreproces::Run()
{
	std::vector<std::thread*>& threadVec = m_pTCPClient->GetSockeThreadVec();
	threadVec.push_back(new std::thread(&PlayerPreproces::HandlerExecuteDB, this));
	return true;
}

// 数据库执行
void PlayerPreproces::HandlerExecuteDB()
{
	COUT_LOG(LOG_CINFO, "PlayerPreproces::HandlerDBSave thread begin...");
	while (m_pTCPClient->GetRuninged())
	{
		//进入挂起状态
		std::unique_lock<std::mutex> uniqLock(m_cond.GetMutex());
		m_cond.Wait(uniqLock, [this] { if (this->m_sqlList.size() > 0) { return true; } return false; });

		if (this->m_sqlList.size() <= 0)
		{
			continue;
		}

		std::string sql = m_sqlList.front();
		m_sqlList.pop_front();

		uniqLock.unlock();

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
		COUT_LOG(LOG_CERROR, "网络链接关闭");
		return;
	}
	unsigned int uMainID = pPlayerInfo->m_pMsg->netMessageHead.uMainID;
	if (uMainID >= (unsigned int)MsgCmd::MsgCmd_End || uMainID <= (unsigned int)MsgCmd::MsgCmd_Begin)
	{
		COUT_LOG(LOG_CERROR, "没有找到住消息类型 cmd = %d", uMainID);
		return;
	}
	// websocket服务器
	if (pPlayerInfo->m_pMsg->socketType == SocketType::SOCKET_TYPE_WEBSOCKET)
	{

	}
	else // TCP socket
	{
		if (uMainID == (unsigned int)MsgCmd::MsgCmd_RegisterAccount)
		{
			CallBackFun((MsgCmd)uMainID, pPlayerInfo);
		}
		else
		{
			DispatchMessage((MsgCmd)uMainID, pPlayerInfo);
		}
	}
}

// 分发消息
void PlayerPreproces::DispatchMessage(MsgCmd cmd, PlayerInfo* pPlayerInfo)
{
	m_PlayerCenter.DispatchMessage(cmd, pPlayerInfo);
}

// 创建角色
bool PlayerPreproces::CreatePlayr()
{
	return m_PlayerCenter.CreatePlayr();
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

// 获取玩家管理
PlayerCenter& PlayerPreproces::GetPlayerCenter()
{
	return m_PlayerCenter;
}

// 获取回调函数map
PlayerPreproces::CallBackFunMap& PlayerPreproces::GetCallBackFunMap()
{
	return m_CallBackFunMap;
}

// 加入回调函数
void PlayerPreproces::AddCallBackFun(MsgCmd cmd, std::function<void(PlayerInfo*)>&& fun)
{
	m_CallBackFunMap.insert(std::make_pair(cmd, fun));
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