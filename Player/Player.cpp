#include "../Game/stdafx.h"

Player::Player(const unsigned int& index, const TCPSocketInfo* pSockInfo, const uint64_t& userId) :
	m_pTcpSockInfo(pSockInfo),
	m_userId(userId),
	m_SubPlayerPreproces(nullptr),
	m_index(index),
	m_load(false)
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
		COUT_LOG(LOG_CINFO, "Player preproces is null index = %d, userid = %lld", index, m_userId);
		return false;
	}
	TCPClient* pTCPClient = m_SubPlayerPreproces->GetTCPClient();
	if (!pTCPClient)
	{
		COUT_LOG(LOG_CINFO, "TCP Clien is null index = %d, userid = %lld", index, m_userId);
		return false;
	}
	return pTCPClient->SendData(index, pData, size, mainID, assistID, handleCode, pBufferevent, uIdentification);
}

void Player::DispatchMessage(MsgCmd cmd, PlayerInfo* pPlayerInfo)
{
	CallBackFun(cmd, pPlayerInfo);
}

// 获取玩家id
uint64_t Player::GetUserId() const
{
	return m_userId;
}

// 获取玩家信息
const TCPSocketInfo* Player::GetTCPSocketInfo()
{
	return m_pTcpSockInfo;
}

// 加入回调函数
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

// 回调函数
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

void Player::AddMysqlCallback(std::string name, std::function<void(std::string&&)>&& fun)
{
	MysqlFunMap::iterator it = m_MysqlCBFunMap.find(name);
	if (it == m_MysqlCBFunMap.end())
	{
		m_MysqlCBFunMap.insert(std::make_pair(name, fun));
		return;
	}

	COUT_LOG(LOG_CINFO, "There is already a callback for this message. Please check the code cmd = %s", name.c_str());
}

bool Player::CallBackFun()
{
	for (MysqlFunMap::iterator it = m_MysqlCBFunMap.begin(); it != m_MysqlCBFunMap.end(); ++it)
	{
		it->second(LoadOneSql(it->first));
	}
	
	return true;
}

// 数据库操作
// 加载一条数据库
std::string Player::LoadOneSql(std::string sqlName, std::string dataStr)
{
	if (!m_SubPlayerPreproces)
	{
		COUT_LOG(LOG_CERROR, "Player preproces is null userid = %lld", m_userId);
		return "";
	}
	return m_SubPlayerPreproces->LoadOneSql(sqlName, m_userId, dataStr);
}

// insert mysql
void Player::SaveInsertSQL(std::string sqlName, std::string data, std::string keyName, std::string dataName)
{
	if (!m_SubPlayerPreproces)
	{
		COUT_LOG(LOG_CINFO, "Player preproces is null userid = %lld", m_userId);
		return;
	}

	m_SubPlayerPreproces->SaveInsertSQL(sqlName, m_userId, data, keyName, dataName);
}

// delete mysql
void Player::SaveDeleteSQL(std::string sqlName, const std::string& sCondition)
{
	if (!m_SubPlayerPreproces)
	{
		COUT_LOG(LOG_CINFO, "Player preproces is null userid = %lld", m_userId);
		return;
	}

	m_SubPlayerPreproces->SaveDeleteSQL(sqlName, sCondition);
}

// replace mysql
void Player::SaveReplaceSQL(std::string sqlName, std::string data, std::string keyName, std::string dataName)
{
	if (!m_SubPlayerPreproces)
	{
		COUT_LOG(LOG_CINFO, "Player preproces is null userid = %lld", m_userId);
		return;
	}

	m_SubPlayerPreproces->SaveReplaceSQL(sqlName, m_userId, data, keyName, dataName);
}

// update mysql
void Player::SaveUpdateSQL(std::string sqlName, std::string data, const std::string& sCondition, std::string keyName, std::string dataName)
{
	if (!m_SubPlayerPreproces)
	{
		COUT_LOG(LOG_CINFO, "Player preproces is null userid = %lld", m_userId);
		return;
	}

	m_SubPlayerPreproces->SaveUpdateSQL(sqlName, m_userId, data, sCondition, keyName, dataName);
}

// 加载数据库
void Player::LoadMysql()
{
	CallBackFun();
}

bool Player::EnterScene()
{
	return true;
}

// 进入游戏
void Player::EnterGame()
{

}

// 玩家退出
void Player::ExitGame()
{
	m_load = false;
}

void Player::SetPlayerPreproces(SubPlayerPreproces* pp)
{ 
	m_SubPlayerPreproces = pp; 
}

void Player::SetLoad(bool load)
{
	m_load = load;
}

int Player::GetIndex() 
{ 
	return m_index; 
}

bool Player::GetLoad() 
{ 
	return m_load; 
}