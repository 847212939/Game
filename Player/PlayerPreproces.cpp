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

// ��ʼ����Ϣ�ص�����
bool PlayerPreproces::intCallBackFun()
{
	// ��¼ע��ص�����
	AddCallBackFun(MsgCmd::MsgCmd_RegisterAccount, std::move(std::bind(&PlayerPreproces::RegisterAccount, this, std::placeholders::_1)));
	return true;
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
		COUT_LOG(LOG_CERROR, "�������ӹر�");
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
		COUT_LOG(LOG_CERROR, "�������ӹر�");
		return;
	}
	unsigned int uMainID = pPlayerInfo->m_pMsg->netMessageHead.uMainID;
	if (uMainID >= (unsigned int)MsgCmd::MsgCmd_End || uMainID <= (unsigned int)MsgCmd::MsgCmd_Begin)
	{
		COUT_LOG(LOG_CERROR, "û���ҵ�ס��Ϣ���� cmd = %d", uMainID);
		return;
	}
	// websocket������
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

// �ַ���Ϣ
void PlayerPreproces::DispatchMessage(MsgCmd cmd, PlayerInfo* pPlayerInfo)
{
	m_PlayerCenter.DispatchMessage(cmd, pPlayerInfo);
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

// ��ȡ��ҹ���
PlayerCenter& PlayerPreproces::GetPlayerCenter()
{
	return m_PlayerCenter;
}

// ��ȡ�ص�����map
PlayerPreproces::CallBackFunMap& PlayerPreproces::GetCallBackFunMap()
{
	return m_CallBackFunMap;
}

// ����ص�����
void PlayerPreproces::AddCallBackFun(MsgCmd cmd, std::function<void(PlayerInfo*)>&& fun)
{
	m_CallBackFunMap.insert(std::make_pair(cmd, fun));
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