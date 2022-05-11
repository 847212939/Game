#include "../Game/stdafx.h"

PlayerPreproces::PlayerPreproces(TCPClient* pTCPClient) : m_pTCPClient(pTCPClient)
{
	InitDB();
	Run();
}

PlayerPreproces::~PlayerPreproces()
{

}

// �������ݿ�
bool PlayerPreproces::InitDB()
{
	// �������ݿ�
	m_CMysqlHelper.init("127.0.0.1", "root", "", "game", "", 3366);
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

// ע���˺�
bool PlayerPreproces::RegisterAccount(std::string& id, std::string& passwaed)
{
	return true;
}

// ����˺���Ϣ
bool PlayerPreproces::CheckUserAccount(std::string& id, std::string& passwaed)
{
	if (id.empty() || passwaed.empty())
	{
		// �˺Ż����������
		// send
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
			// �˺�������֤ͨ��
			// send
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
			m_accountMap[id] = pw;
			return false;
		}
		else
		{
			// �˺�������֤ͨ��
			// send
			m_accountMap[id] = pw;
			return true;
		}
	}
}

// ��������˺���Ϣ
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

// ���ݿ�ִ��
void PlayerPreproces::HandlerExecuteDB()
{
	COUT_LOG(LOG_CINFO, "PlayerPreproces::HandlerDBSave thread begin...");
	while (m_pTCPClient->GetRuninged())
	{
		//�������״̬
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
			COUT_LOG(LOG_CERROR, "ִ�����ݿ�ʧ��:%s", excep.errorInfo.c_str());
		}
	}
}

// ������Ϣ
void PlayerPreproces::HandlerMessage(PlayerInfo* pInfo)
{
	// websocket������
	if (pInfo->uSrverType == SocketType::SOCKET_TYPE_WEBSOCKET)
	{

	}
	else // ����������
	{

	}
}

// �ַ���Ϣ
void PlayerPreproces::DispatchMessage()
{
	m_PlayerCenter.DispatchMessage();
}

// ������ɫ
bool PlayerPreproces::CreatePlayr()
{
	return m_PlayerCenter.CreatePlayr();
}

// ��ȡ֪ͨ��������
ConditionVariable& PlayerPreproces::GetConditionVariable()
{
	return m_cond;
}

// insert mysql
bool PlayerPreproces::SaveInsertSQL(std::string sqlName, std::string name, std::string data, std::string keyName/* = "userid"*/, std::string dataName/* = "data"*/)
{
	CMysqlHelper::RECORD_DATA mpColumns;

	mpColumns.insert(std::make_pair(keyName, std::make_pair(CMysqlHelper::FT::DB_STR, name)));
	mpColumns.insert(std::make_pair(dataName, std::make_pair(CMysqlHelper::FT::DB_STR, data)));

	std::string sSql = m_CMysqlHelper.buildInsertSQL(sqlName, mpColumns);

	m_cond.GetMutex().lock();
	m_sqlList.push_back(sSql);
	m_cond.GetMutex().unlock();

	m_cond.NotifyOne();

	return true;
}

// update mysql
bool PlayerPreproces::SaveUpdateSQL(std::string sqlName, std::string name, std::string data, const std::string& sCondition, std::string keyName/* = "userid"*/, std::string dataName/* = "data"*/)
{
	CMysqlHelper::RECORD_DATA mpColumns;

	mpColumns.insert(std::make_pair(keyName, std::make_pair(CMysqlHelper::FT::DB_STR, name)));
	mpColumns.insert(std::make_pair(dataName, std::make_pair(CMysqlHelper::FT::DB_STR, data)));

	std::string sSql = m_CMysqlHelper.buildUpdateSQL(sqlName, mpColumns, sCondition);

	m_cond.GetMutex().lock();
	m_sqlList.push_back(sSql);
	m_cond.GetMutex().unlock();

	m_cond.NotifyOne();

	return true;
}

// Replace mysql
bool PlayerPreproces::SaveReplaceSQL(std::string sqlName, std::string name, std::string data, std::string keyName/* = "userid"*/, std::string dataName/* = "data"*/)
{
	CMysqlHelper::RECORD_DATA mpColumns;

	mpColumns.insert(std::make_pair(keyName, std::make_pair(CMysqlHelper::FT::DB_STR, name)));
	mpColumns.insert(std::make_pair(dataName, std::make_pair(CMysqlHelper::FT::DB_STR, data)));

	std::string sSql = m_CMysqlHelper.buildReplaceSQL(sqlName, mpColumns);

	m_cond.GetMutex().lock();
	m_sqlList.push_back(sSql);
	m_cond.GetMutex().unlock();

	m_cond.NotifyOne();

	return true;
}

// delete mysql
bool PlayerPreproces::SaveDeleteSQL(std::string sqlName, const std::string& sCondition)
{
	std::ostringstream sSql;
	sSql << "delete from " << sqlName << " " << sCondition;

	m_cond.GetMutex().lock();
	m_sqlList.push_back(sSql.str());
	m_cond.GetMutex().unlock();

	m_cond.NotifyOne();

	return true;
}