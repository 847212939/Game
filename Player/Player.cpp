#include "../Game/stdafx.h"

Player::Player(TCPSocketInfo* sockInfo) : m_userId(0), m_TCPSocketInfo(sockInfo)
{

}

Player::~Player()
{

}

// 获取玩家id
Player::UserId Player::GetUserId()
{
	return m_userId;
}

// 获取玩家链接信息
TCPSocketInfo* Player::GetTCPSocketInfo()
{
	if (!m_TCPSocketInfo)
	{
		return nullptr;
	}
	return m_TCPSocketInfo;
}