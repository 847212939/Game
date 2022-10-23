#include "../stdafx.h"

Player::Player(const unsigned int& index) :
	Animal(),
	m_Index(index),
	m_Load(false),
	m_Playername("")
{
}
Player::~Player()
{
}

AnimalType Player::GetType()
{
	return AnimalType::at_player;
}

bool Player::SendMsg(const char* pData, size_t size, MsgCmd mainID, int assistID, int handleCode, unsigned int uIdentification)
{
	const TCPSocketInfo* pInfo = DTCPC->GetTCPSocketInfo(m_Index);
	if (!pInfo)
	{
		COUT_LOG(LOG_CERROR, "Client information is empty index = %d", m_Index);
		return false;
	}
	return DTCPC->SendMsg(m_Index, pData, size, mainID, assistID, handleCode, pInfo->bev, uIdentification);
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

	COUT_LOG(LOG_CERROR, "There is already a callback for this message. Please check the code cmd = %d", cmd);
}
void Player::AddMysqlCallback(SLoadMysql loadMysql)
{
	MapMysqlFunc::iterator it = m_MysqlCBFunMap.find(loadMysql.uMainID);
	if (it == m_MysqlCBFunMap.end())
	{
		m_MysqlCBFunMap.insert(std::make_pair(loadMysql.uMainID, loadMysql));
		return;
	}

	COUT_LOG(LOG_CERROR, "There is already a callback for this message. Please check the code table = %s", loadMysql.sqlName.c_str());
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
		COUT_LOG(LOG_CERROR, "No corresponding callback function found cmd = %d", cmd);
		return;
	}

	it->second(playerInfo);
}
void Player::MysqlCallBackFun()
{
	for (MapMysqlFunc::iterator it = m_MysqlCBFunMap.begin(); it != m_MysqlCBFunMap.end(); ++it)
	{
		DPPC->GetMysqlClient().LoadPlayerMysql(this, it->second);
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
void Player::LoadOneSql(SLoadMysql& loadMysql)
{
	int index = DTCPC->GetDBServerIndex();
	if (index <= 0)
	{
		COUT_LOG(LOG_CERROR, "数据库链接失败");
		return;
	}
	TCPSocketInfo* tcpInfo = DTCPC->GetTCPSocketInfo(index);
	if (!tcpInfo)
	{
		COUT_LOG(LOG_CERROR, "数据库链接失败");
		return;
	}
	Netmsg msg;
	msg << 2 << BaseCfgMgr.GetServerId() << GetID() << loadMysql.sqlName 
		<< loadMysql.uMainID << loadMysql.uAssistantID << loadMysql.uIdentification;
	DTCPC->SendMsg(index, msg.str().c_str(), msg.str().size(), 
		MsgCmd::MsgCmd_DBServer, 1, 0, tcpInfo->bev, (unsigned int)MsgCmd::MsgCmd_DBServer);
}
void Player::SaveInsertSQL(std::string sqlName, std::string data, std::string keyName, std::string dataName)
{
	DPPC->SaveInsertSQL(sqlName, GetID(), data, keyName, dataName);
}
void Player::SaveDeleteSQL(std::string sqlName, const std::string& sCondition)
{
	DPPC->SaveDeleteSQL(sqlName, sCondition);
}
void Player::SaveReplaceSQL(std::string sqlName, std::string data, std::string keyName, std::string dataName)
{
	int index = DTCPC->GetDBServerIndex();
	if (index <= 0)
	{
		COUT_LOG(LOG_CERROR, "数据库链接失败");
		return;
	}
	TCPSocketInfo* tcpInfo = DTCPC->GetTCPSocketInfo(index);
	if (!tcpInfo)
	{
		COUT_LOG(LOG_CERROR, "数据库链接失败");
		return;
	}
	/*DTCPC->SendMsg(index, "", 0, (MsgCmd)loadMysql.uMainID, loadMysql.uAssistantID,
		2, tcpInfo->bev, loadMysql.uIdentification);
	DPPC->SaveReplaceSQL(sqlName, GetID(), data, keyName, dataName);*/
}
void Player::SaveUpdateSQL(std::string sqlName, std::string data, const std::string& sCondition, std::string keyName, std::string dataName)
{
	DPPC->SaveUpdateSQL(sqlName, GetID(), data, sCondition, keyName, dataName);
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
	DSC->EnterScene(this, GetSceneid(), Gettransform());
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
