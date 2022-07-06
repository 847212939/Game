#include "../Game/stdafx.h"

Player::Player(const TCPSocketInfo* pSockInfo, std::string& userId) : 
	m_pTcpSockInfo(pSockInfo),
	m_userId(userId)
{

}

Player::~Player()
{

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