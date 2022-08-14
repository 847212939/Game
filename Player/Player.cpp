#include "../Game/stdafx.h"

Player::Player(const unsigned int& index) :
	Animal(),
	m_index(index),
	m_load(false)
{
}

Player::~Player()
{
}

AnimalType Player::GetType()
{
	return AnimalType::at_player;
}

bool Player::SendData(const char* pData, size_t size, MsgCmd mainID, int assistID, int handleCode, unsigned int uIdentification)
{
	const TCPSocketInfo* pInfo = DTCPC->GetTCPSocketInfo(m_index);
	if (!pInfo)
	{
		COUT_LOG(LOG_CERROR, "Client information is empty index = %d", m_index);
		return false;
	}
	return DTCPC->SendData(m_index, pData, size, mainID, assistID, handleCode, pInfo->bev, uIdentification);
}

void Player::MessageDispatch(MsgCmd cmd, PlayerInfo* playerInfo)
{
	NetCallBackFun(cmd, playerInfo);
}

void Player::AddExitCallback(std::function<void(SocketCloseLine*)>&& fun)
{
	m_ExitFunMap.push_back(fun);
}

void Player::AddAttrsCallback(std::function<void()>&& fun)
{
	m_AttrsFunMap.push_back(fun);
}

void Player::AddEnterSceneCallback(std::function<void()>&& fun)
{
	m_EnterSceneFunMap.push_back(fun);
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

void Player::AddMysqlCallback(std::string name, std::function<void(std::string&)>&& fun)
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

void Player::NetCallBackFun(MsgCmd cmd, PlayerInfo* playerInfo)
{
	NetFunMap::iterator it = m_NetCBFunMap.find(cmd);
	if (it == m_NetCBFunMap.end())
	{
		COUT_LOG(LOG_CERROR, "No corresponding callback function found cmd = %d", cmd);
		return;
	}

	it->second(playerInfo);
}

void Player::MysqlCallBackFun()
{
	std::string str;

	for (MysqlFunMap::iterator it = m_MysqlCBFunMap.begin(); it != m_MysqlCBFunMap.end(); ++it)
	{
		str.clear();

		LoadOneSql(it->first, str);

		it->second(str);
	}
}

void Player::AttrsCallBackFun()
{
	for (auto& fun : m_AttrsFunMap)
	{
		fun();
	}
}

void Player::EnterSceneCallBackFun()
{
	for (auto& fun : m_EnterSceneFunMap)
	{
		fun();
	}
}

// 数据库操作
// 加载一条数据库
void Player::LoadOneSql(std::string sqlName, std::string& outStr, std::string dataStr)
{
	DPPC->LoadOneSql(sqlName, GetID(), outStr, dataStr);
}

// insert mysql
void Player::SaveInsertSQL(std::string sqlName, std::string data, std::string keyName, std::string dataName)
{
	DPPC->SaveInsertSQL(sqlName, GetID(), data, keyName, dataName);
}

// delete mysql
void Player::SaveDeleteSQL(std::string sqlName, const std::string& sCondition)
{
	DPPC->SaveDeleteSQL(sqlName, sCondition);
}

// replace mysql
void Player::SaveReplaceSQL(std::string sqlName, std::string data, std::string keyName, std::string dataName)
{
	DPPC->SaveReplaceSQL(sqlName, GetID(), data, keyName, dataName);
}

// update mysql
void Player::SaveUpdateSQL(std::string sqlName, std::string data, const std::string& sCondition, std::string keyName, std::string dataName)
{
	DPPC->SaveUpdateSQL(sqlName, GetID(), data, sCondition, keyName, dataName);
}

// 加载数据库
void Player::LoadMysql()
{
	MysqlCallBackFun();
}

// 进入场景
void Player::EnterScene()
{
	EnterSceneCallBackFun();

	// 玩家进入场景
	DSC->EnterScene(this, GetSceneid(), Gettransform());
}

// 进入游戏
void Player::CalAttrs()
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
