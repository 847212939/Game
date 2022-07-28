#include "../Game/stdafx.h"

Player::Player(const unsigned int& index, const TCPSocketInfo* pSockInfo, const uint64_t& userId, SubPlayerPreproces* pp) :
	m_pTcpSockInfo(pSockInfo),
	m_userId(userId),
	m_SubPlayerPreproces(pp),
	m_index(index),
	m_load(false)
{
}

Player::~Player()
{
}

bool Player::SendData(int index, const char* pData, size_t size, MsgCmd mainID, int assistID, int handleCode, void* pBufferevent, unsigned int uIdentification)
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
	NetCallBackFun(cmd, pPlayerInfo);
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

SubPlayerPreproces* Player::GetSubPlayerPreproces()
{
	return m_SubPlayerPreproces;
}

void Player::AddExitCallback(std::function<void(SocketCloseLine*)>&& fun)
{
	m_ExitFunMap.push_back(fun);
}

void Player::AddAttrsCallback(std::function<void()>&& fun)
{
	m_AttrsFunMap.push_back(fun);
}

void Player::AddNetCallback(MsgCmd cmd, std::function<void(PlayerInfo*)>&& fun)
{
	NetFunMap::iterator it = m_NetCBFunMap.find(cmd);
	if (it == m_NetCBFunMap.end())
	{
		m_NetCBFunMap.insert(std::make_pair(cmd, fun));
		return;
	}

	COUT_LOG(LOG_CERROR, "There is already a callback for this message. Please check the code cmd = %d", cmd);
}

void Player::AddMysqlCallback(std::string name, std::function<void(std::string&&)>&& fun)
{
	MysqlFunMap::iterator it = m_MysqlCBFunMap.find(name);
	if (it == m_MysqlCBFunMap.end())
	{
		m_MysqlCBFunMap.insert(std::make_pair(name, fun));
		return;
	}

	COUT_LOG(LOG_CERROR, "There is already a callback for this message. Please check the code table = %s", name.c_str());
}

void Player::ExitCallBackFun(SocketCloseLine* pSocketClose)
{
	for (auto& fun : m_ExitFunMap)
	{
		fun(pSocketClose);
	}
}

void Player::NetCallBackFun(MsgCmd cmd, PlayerInfo* pPlayerInfo)
{
	NetFunMap::iterator it = m_NetCBFunMap.find(cmd);
	if (it == m_NetCBFunMap.end())
	{
		COUT_LOG(LOG_CERROR, "No corresponding callback function found cmd = %d", cmd);
		return;
	}

	it->second(pPlayerInfo);
}

void Player::MysqlCallBackFun()
{
	for (MysqlFunMap::iterator it = m_MysqlCBFunMap.begin(); it != m_MysqlCBFunMap.end(); ++it)
	{
		it->second(LoadOneSql(it->first));
	}
}

void Player::AttrsCallBackFun()
{
	for (auto& fun : m_AttrsFunMap)
	{
		fun();
	}
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

void Player::AdditionAttributes(AttrsMap& attrs)
{
	for (AttrsMap::iterator it = attrs.begin(); it != attrs.end(); ++it)
	{
		if ((AttrsCmd)it->first <= AttrsCmd::AttrsCmd_Begin || (AttrsCmd)it->first >= AttrsCmd::AttrsCmd_End)
		{
			COUT_LOG(LOG_CERROR, "未知属性 请检查AttrsCmd.h头文件 属性为id:%d", it->first);
			continue;
		}
		m_AttrsMap[it->first] += it->second;
	}
}

void Player::RefreshProperties()
{
	COstringstream os;
	os << (int)m_AttrsMap.size();
	for (AttrsMap::const_iterator it = m_AttrsMap.begin(); it != m_AttrsMap.end(); ++it)
	{
		os << it->first << it->second;
	}

	SendData(m_index, os.str().c_str(), os.str().size(), MsgCmd::MsgCmd_RefreshProperties, 1, 0, m_pTcpSockInfo->bev);
}

// 加载数据库
void Player::LoadMysql()
{
	MysqlCallBackFun();
}

bool Player::EnterScene()
{
	return true;
}

// 进入游戏
void Player::EnterGame()
{
	AttrsCallBackFun();
}

// 玩家退出
void Player::ExitGame(SocketCloseLine* pSocketClose)
{
	m_load = false;

	ExitCallBackFun(pSocketClose);
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
