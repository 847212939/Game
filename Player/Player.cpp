#include "../Game/stdafx.h"

Player::Player(const unsigned int& index, const TCPSocketInfo* pSockInfo, const std::string& userId) :
	m_pTcpSockInfo(pSockInfo),
	m_userId(userId),
	m_SubPlayerPreproces(nullptr),
	m_index(index)
{
	m_NetCBFunMap.clear();
}

Player::~Player()
{
}

bool Player::SendData(int index, void* pData, int size, int mainID, int assistID, int handleCode, void* pBufferevent, unsigned int uIdentification)
{
	if (!m_SubPlayerPreproces)
	{
		COUT_LOG(LOG_CINFO, "Player preproces is null index = %d, userid = %s", index, m_userId.c_str());
		return false;
	}
	TCPClient* pTCPClient = m_SubPlayerPreproces->GetTCPClient();
	if (!pTCPClient)
	{
		COUT_LOG(LOG_CINFO, "TCP Clien is null index = %d, userid = %s", index, m_userId.c_str());
		return false;
	}
	return pTCPClient->SendData(index, pData, size, mainID, assistID, handleCode, pBufferevent, uIdentification);
}

void Player::DispatchMessage(MsgCmd cmd, PlayerInfo* pPlayerInfo)
{
	CallBackFun(cmd, pPlayerInfo);
}

// ��ȡ���id
std::string Player::GetUserId() const
{
	return m_userId;
}

// ��ȡ�����Ϣ
const TCPSocketInfo* Player::GetTCPSocketInfo()
{
	return m_pTcpSockInfo;
}

// ����ص�����
void Player::AddNetCallback(MsgCmd cmd, std::function<void(PlayerInfo*)>&& fun)
{
	NetFunMap::iterator it = m_NetCBFunMap.find(cmd);
	if (it == m_NetCBFunMap.end())
	{
		m_NetCBFunMap.insert(std::make_pair(cmd, fun));
		return;
	}

	COUT_LOG(LOG_CINFO, "There is already a callback for this message. Please check the code cmd = %d", cmd);
}

// �ص�����
bool Player::CallBackFun(MsgCmd cmd, PlayerInfo* pPlayerInfo)
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

void Player::AddMysqlCallback(std::pair<std::string, std::string>&& pr, std::function<void(std::string&)>&& fun)
{
	MysqlFunMap::iterator it = m_MysqlCBFunMap.find(pr);
	if (it == m_MysqlCBFunMap.end())
	{
		m_MysqlCBFunMap.insert(std::make_pair(pr, fun));
		return;
	}

	COUT_LOG(LOG_CINFO, "There is already a callback for this message. Please check the code cmd = %s", pr.first.c_str());
}

bool Player::CallBackFun()
{
	for (MysqlFunMap::iterator it = m_MysqlCBFunMap.begin(); it != m_MysqlCBFunMap.end(); ++it)
	{
		CMysqlHelper::MysqlData mysqlData;
		LoadOneSql(it->first.first, it->first.second, mysqlData);
		if (mysqlData.size() <= 0)
		{
			continue;
		}

		SqlKeyDataMap& dataMap = mysqlData[0];
		SqlKeyDataMap::iterator pos = dataMap.find("data");
		if (pos == dataMap.end())
		{
			continue;
		}

		it->second(pos->second);
	}
	
	return true;
}

// ���ݿ����
// ����һ�����ݿ�
bool Player::LoadOneSql(std::string userId, std::string sqlName, CMysqlHelper::MysqlData& queryData, std::string dataStr)
{
	if (!m_SubPlayerPreproces)
	{
		COUT_LOG(LOG_CINFO, "Player preproces is null userid = %s", m_userId.c_str());
		return false;
	}
	return m_SubPlayerPreproces->LoadOneSql(userId, sqlName, queryData, dataStr);
}

// ���ض������ݿ�
bool Player::LoadMulitySql(std::string userId, std::string sqlName, CMysqlHelper::MysqlData& queryData, std::string dataStr)
{
	if (!m_SubPlayerPreproces)
	{
		COUT_LOG(LOG_CINFO, "Player preproces is null userid = %s", m_userId.c_str());
		return false;
	}

	return m_SubPlayerPreproces->LoadMulitySql(userId, sqlName, queryData, dataStr);
}

// insert mysql
void Player::SaveInsertSQL(std::string sqlName, std::string name, std::string data, std::string keyName, std::string dataName)
{
	if (!m_SubPlayerPreproces)
	{
		COUT_LOG(LOG_CINFO, "Player preproces is null userid = %s", m_userId.c_str());
		return;
	}

	m_SubPlayerPreproces->SaveInsertSQL(sqlName, name, data, keyName, dataName);
}

// delete mysql
void Player::SaveDeleteSQL(std::string sqlName, const std::string& sCondition)
{
	if (!m_SubPlayerPreproces)
	{
		COUT_LOG(LOG_CINFO, "Player preproces is null userid = %s", m_userId.c_str());
		return;
	}

	m_SubPlayerPreproces->SaveDeleteSQL(sqlName, sCondition);
}

// replace mysql
void Player::SaveReplaceSQL(std::string sqlName, std::string name, std::string data, std::string keyName, std::string dataName)
{
	if (!m_SubPlayerPreproces)
	{
		COUT_LOG(LOG_CINFO, "Player preproces is null userid = %s", m_userId.c_str());
		return;
	}

	m_SubPlayerPreproces->SaveReplaceSQL(sqlName, name, data, keyName, dataName);
}

// update mysql
void Player::SaveUpdateSQL(std::string sqlName, std::string name, std::string data, const std::string& sCondition, std::string keyName, std::string dataName)
{
	if (!m_SubPlayerPreproces)
	{
		COUT_LOG(LOG_CINFO, "Player preproces is null userid = %s", m_userId.c_str());
		return;
	}

	m_SubPlayerPreproces->SaveUpdateSQL(sqlName, name, data, sCondition, keyName, dataName);
}

// �������ݿ�
void Player::LoadMysql()
{
	CallBackFun();
}

bool Player::EnterScene()
{
	return true;
}

// ������Ϸ
void Player::EnterGame()
{

}

// ����˳�
void Player::ExitGame()
{

}
