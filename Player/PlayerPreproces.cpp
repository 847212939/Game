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

// ��ʼ����Ϣ�ص�����
void PlayerPreproces::initCallBackFun()
{
	// ��¼ע��ص�����
	AddCallBackFun(MsgCmd::MsgCmd_Login, std::move(std::bind(&PlayerPreproces::LoginInAccount, this, std::placeholders::_1)));
}

// ��¼
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
		COUT_LOG(LOG_CERROR, "�������ӹر�");
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

// ע���˺�
bool PlayerPreproces::Register(std::string& id, std::string& passwaed, PlayerInfo* pPlayerInfo)
{
	if (id.empty() || passwaed.empty())
	{
		// �˺Ų�����
		return false;
	}

	std::string userId = Util::CreateUuid();

	m_accountMap.insert(std::make_pair(id, passwaed));
	m_AccountUserIDMap.insert(std::make_pair(id, userId));

	// �������ݿ�
	SaveReplaceSQL("useraccount", id, passwaed);
	SaveReplaceSQL("userid", id, userId);
	return true;
}

// ����˺��Ƿ����
bool PlayerPreproces::LoginIn(std::string& id, std::string& passwaed, PlayerInfo* pPlayerInfo)
{
	if (id.empty() || passwaed.empty())
	{
		// �˺Ų�����
		return false;
	}
	AccountMap::const_iterator it = m_accountMap.find(id);
	if (it != m_accountMap.end())
	{
		// �ڴ��ѯ
		if (it->second != passwaed)
		{
			// �������
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
		// ���ݿ��ѯ
		std::string pw = LoadUserAccount(id);
		if (pw.empty())
		{
			// �˺Ŵ���
			// send
			return false;
		}
		if (pw != passwaed)
		{
			// �������
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

// �������userid
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

// ��������˺���Ϣ
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

// �������ݿ�
bool PlayerPreproces::InitDB()
{
	CBaseCfgMgr& baseCfgMgr = CfgMgr()->GetCBaseCfgMgr();
	const DbCfg& dbCfg = baseCfgMgr.GetDbCfg();

	// �������ݿ�
	m_CMysqlHelper.init(dbCfg.ip.c_str(), dbCfg.user.c_str(), dbCfg.passwd.c_str(), dbCfg.database.c_str(), "", dbCfg.port);
	try
	{
		m_CMysqlHelper.connect();
	}
	catch (MysqlHelper_Exception& excep)
	{
		COUT_LOG(LOG_CERROR, "�������ݿ�ʧ��:%s", excep.errorInfo.c_str());
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

// ���ݿ�ִ��
void PlayerPreproces::HandlerExecuteSqlThread()
{
	COUT_LOG(LOG_CINFO, "PlayerPreproces::HandlerDBSave thread begin...");
	std::this_thread::sleep_for(std::chrono::seconds(2));

	while (m_pTCPClient->GetRuninged())
	{
		//�������״̬
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
				COUT_LOG(LOG_CERROR, "ִ�����ݿ�ʧ��:%s", excep.errorInfo.c_str());
			}
		}
	}
	COUT_LOG(LOG_CINFO, "PlayerPreproces::HandlerDBSave thread end...");
}

// ������Ϣ
void PlayerPreproces::HandlerMessage(PlayerInfo* pPlayerInfo)
{
	if (!pPlayerInfo->m_pMsg || !pPlayerInfo->m_pTcpSockInfo)
	{
		COUT_LOG(LOG_CERROR, "!pPlayerInfo->pMsg || !pPlayerInfo->pTcpSockInfo");
		return;
	}
	if (!pPlayerInfo->m_pTcpSockInfo->isConnect)
	{
		COUT_LOG(LOG_CERROR, "�������ӹر�");
		return;
	}
	unsigned int uMainID = pPlayerInfo->m_pMsg->netMessageHead.uMainID;
	if (uMainID >= (unsigned int)MsgCmd::MsgCmd_End || uMainID <= (unsigned int)MsgCmd::MsgCmd_Begin)
	{
		COUT_LOG(LOG_CERROR, "û���ҵ���Ϣ���� cmd = %d", uMainID);
		return;
	}
	// websocket������
	if (pPlayerInfo->m_pMsg->socketType == SocketType::SOCKET_TYPE_WEBSOCKET)
	{

	}
	else if (pPlayerInfo->m_pMsg->socketType == SocketType::SOCKET_TYPE_TCP)
	{
		DispatchMessage((MsgCmd)uMainID, pPlayerInfo);
	}
}

// �ַ���Ϣ
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

// ������ɫ
bool PlayerPreproces::CreatePlayr(PlayerInfo* pPlayerInfo)
{
	return m_scene.CreatePlayr(pPlayerInfo);
}

// ��ȡ֪ͨ��������
ConditionVariable& PlayerPreproces::GetConditionVariable()
{
	return m_cond;
}

// ��ȡ����˻���Ϣ
PlayerPreproces::AccountMap& PlayerPreproces::GetAccountMap()
{
	return m_accountMap;
}

// ��ȡ������
TCPClient* PlayerPreproces::GetTCPClient()
{
	if (m_pTCPClient)
	{
		return m_pTCPClient;
	}

	return nullptr;
}

// ��ȡ���ݿ�
CMysqlHelper& PlayerPreproces::GetCMysqlHelper()
{
	return m_CMysqlHelper;
}

// ��ȡ����
Scene& PlayerPreproces::GetScene()
{
	return m_scene;
}

// ��ȡ�ص�����map
PlayerPreproces::CallBackFunMap& PlayerPreproces::GetCallBackFunMap()
{
	return m_CallBackFunMap;
}

// ����ص�����
void PlayerPreproces::AddCallBackFun(MsgCmd cmd, std::function<void(PlayerInfo*)>&& fun)
{
	CallBackFunMap::iterator it = m_CallBackFunMap.find(cmd);
	if (it == m_CallBackFunMap.end())
	{
		m_CallBackFunMap.insert(std::make_pair(cmd, fun));
		return;
	}

	COUT_LOG(LOG_CINFO, "�Ѿ����ڸ���Ϣ�Ļص�������� cmd = %d", cmd);
}

// �ص�����
bool PlayerPreproces::CallBackFun(MsgCmd cmd, PlayerInfo* pPlayerInfo)
{
	CallBackFunMap::iterator it = m_CallBackFunMap.find(cmd);
	if (it == m_CallBackFunMap.end())
	{
		COUT_LOG(LOG_CERROR, "û�в��ҵ���Ӧ�Ļص����� cmd = %d", cmd);
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

// ����һ�����ݿ�
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
		COUT_LOG(LOG_CERROR, "�������ݿ�ʧ��:%s userId = %s", excep.errorInfo.c_str(), userId.c_str());
		return false;
	}

	return true;
}

// ���ض������ݿ�
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
		COUT_LOG(LOG_CERROR, "�������ݿ�ʧ��:%s userId = %s", excep.errorInfo.c_str(), userId.c_str());
		return false;
	}

	return true;
}