#include "../stdafx.h"

Player::Player(const unsigned int& index) :
	Animal(),
	m_Index(index),
	m_Load(false),
	m_Playername(""),
	m_LogicIndex(-1),
	m_Serverid(0)
{
}
Player::~Player()
{
}

AnimalType Player::GetType()
{
	return AnimalType::at_player;
}
bool Player::SendLogicMsg(const char* pData, size_t size, MsgCmd mainID, int assistID, int handleCode, unsigned int uIdentification/* = 0*/)
{
	const TCPSocketInfo* pInfo = G_NetClient->GetTCPSocketInfo(m_Index);
	if (!pInfo)
	{
		Log(CERR, "Client information is empty index = %d", m_Index);
		return false;
	}
	return G_NetClient->SendMsg(m_Index, pData, size, mainID, assistID, handleCode, pInfo->bev, uIdentification, GetID());
}
bool Player::SendCrossMsg(const char* pData, size_t size, MsgCmd mainID, int assistID, int handleCode, unsigned int uIdentification/* = 0*/)
{
	const TCPSocketInfo* pCrossTcpInfo = G_NetClient->GetTCPSocketInfo(m_Index);
	if (!pCrossTcpInfo)
	{
		Log(CERR, "Client information is empty index = %d", m_Index);
		return false;
	}
	return G_NetClient->SendMsg(m_Index, pData, size, mainID, assistID, handleCode, pCrossTcpInfo->bev, uIdentification, GetID());
}
bool Player::SendMsg(const char* pData, size_t size, MsgCmd mainID, int assistID, int handleCode, unsigned int uIdentification)
{
	return G_NetClient->GetServerType() ==
		ServiceType::SERVICE_TYPE_CROSS ?
		SendCrossMsg(pData, size, mainID, assistID, handleCode, uIdentification):
		SendLogicMsg(pData, size, mainID, assistID, handleCode, uIdentification);
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
	MapNetFun::iterator it = m_NetCBFunMap.find(cmd);
	if (it == m_NetCBFunMap.end())
	{
		m_NetCBFunMap.insert(std::make_pair(cmd, fun));
		return;
	}

	Log(CERR, "There is already a callback for this message. Please check the code cmd = %d", cmd);
}
void Player::AddMysqlCallback(SLoadMysql loadMysql)
{
	TCPSocketInfo* pTcpInfo = G_NetClient->GetTCPSocketInfo(m_Index);
	if (!pTcpInfo)
	{
		return;
	}
	if (pTcpInfo->isCross)
	{
		return;
	}
	MapMysqlFunc::iterator it = m_MysqlCBFunMap.find(loadMysql.uMainID);
	if (it == m_MysqlCBFunMap.end())
	{
		m_MysqlCBFunMap.insert(std::make_pair(loadMysql.uMainID, loadMysql));
		return;
	}

	Log(CERR, "There is already a callback for this message. Please check the code table = %s", loadMysql.sqlName.c_str());
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
	MapNetFun::iterator it = m_NetCBFunMap.find(cmd);
	if (it == m_NetCBFunMap.end())
	{
		Log(CERR, "No corresponding callback function found cmd = %d", cmd);
		return;
	}

	it->second(playerInfo);
}
void Player::MysqlCallBackFun()
{
	for (MapMysqlFunc::iterator it = m_MysqlCBFunMap.begin(); it != m_MysqlCBFunMap.end(); ++it)
	{
		MysqlClient::LoadPlayerMysql(GetID(), it->second);
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

// 回调函数
void Player::LoadMysql()
{
	// 加载数据库
	MysqlCallBackFun();
}
void Player::EnterScene()
{
	// 进入场景
	EnterSceneCallBackFun();

	// 玩家进入场景
	G_SceneClient->EnterScene(this, GetSceneid(), Gettransform());
}
void Player::CalAttrs()
{
	// 进入游戏
	AttrsCallBackFun();
}
void Player::ExitGame(SocketCloseLine* pSocketClose)
{
	// 玩家退出
	SetLoad(false);

	ExitCallBackFun(pSocketClose);
}
