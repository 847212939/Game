#include "../Game/stdafx.h"

Player::Player(const unsigned int& index, const TCPSocketInfo* pSockInfo, const std::string& userId) :
	m_pTcpSockInfo(pSockInfo),
	m_userId(userId),
	m_SubPlayerPreproces(nullptr),
	m_index(index)
{
	m_CallBackFunMap.clear();
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

// 加载数据库
void Player::LoadMysql()
{

}

bool Player::EnterScene()
{
	return true;
}

// 进入游戏
void Player::EnterGame()
{

}

// 获取玩家id
std::string Player::GetUserId() const
{
	return m_userId;
}

// 获取玩家信息
const TCPSocketInfo* Player::GetTCPSocketInfo()
{
	return m_pTcpSockInfo;
}

// 加入回调函数
void Player::AddCallBackFun(MsgCmd cmd, std::function<void(PlayerInfo*)>&& fun)
{
	CallBackFunMap::iterator it = m_CallBackFunMap.find(cmd);
	if (it == m_CallBackFunMap.end())
	{
		m_CallBackFunMap.insert(std::make_pair(cmd, fun));
		return;
	}

	COUT_LOG(LOG_CINFO, "There is already a callback for this message. Please check the code cmd = %d", cmd);
}

// 回调函数
bool Player::CallBackFun(MsgCmd cmd, PlayerInfo* pPlayerInfo)
{
	CallBackFunMap::iterator it = m_CallBackFunMap.find(cmd);
	if (it == m_CallBackFunMap.end())
	{
		COUT_LOG(LOG_CERROR, "No corresponding callback function found cmd = %d", cmd);
		return false;
	}

	it->second(pPlayerInfo);
	return true;
}

// 数据库操作
// 加载一条数据库
bool Player::LoadOneSql(std::string userId, std::string sqlName, CMysqlHelper::MysqlData& queryData, std::string dataStr)
{
	if (!m_SubPlayerPreproces)
	{
		COUT_LOG(LOG_CINFO, "Player preproces is null userid = %s", m_userId.c_str());
		return false;
	}
	return m_SubPlayerPreproces->LoadOneSql(userId, sqlName, queryData, dataStr);
}

// 加载多条数据库
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