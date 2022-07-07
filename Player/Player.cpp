#include "../Game/stdafx.h"

Player::Player(unsigned int index, const TCPSocketInfo* pSockInfo, std::string& userId) :
	m_pTcpSockInfo(pSockInfo),
	m_userId(userId),
	m_PlayerPreproces(nullptr),
	m_index(index)
{
	m_CallBackFunMap.clear();
}

Player::~Player()
{
}

// 初始化回调函数
void Player::CallBackFunInit()
{
	AddCallBackFun(MsgCmd::MsgCmd_Move, std::move(std::bind(&Player::Move, this, std::placeholders::_1)));
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

bool Player::Move(PlayerInfo* pPlayerInfo)
{
	unsigned int uAssistantID = pPlayerInfo->m_pMsg->netMessageHead.uAssistantID;
	std::string str = (char*)pPlayerInfo->m_pData;
	CIstringstream is(str);
	unsigned int x = 0, y = 0;
	is >> x >> y;

	COUT_LOG(LOG_CINFO, "x = %u, y = %u", x, y);

	return true;
}

// 加入回调函数
void Player::AddCallBackFun(MsgCmd cmd, std::function<void(PlayerInfo*)>&& fun)
{
	PlayerCallBackFunMap::iterator it = m_CallBackFunMap.find(cmd);
	if (it == m_CallBackFunMap.end())
	{
		m_CallBackFunMap.insert(std::make_pair(cmd, fun));
		return;
	}

	COUT_LOG(LOG_CINFO, "已经存在该消息的回调请检查代码 cmd = %d", cmd);
}

// 回调函数
bool Player::CallBackFun(MsgCmd cmd, PlayerInfo* pPlayerInfo)
{
	PlayerCallBackFunMap::iterator it = m_CallBackFunMap.find(cmd);
	if (it == m_CallBackFunMap.end())
	{
		COUT_LOG(LOG_CERROR, "没有查找到相应的回调函数 cmd = %d", cmd);
		return false;
	}

	it->second(pPlayerInfo);
	return true;
}