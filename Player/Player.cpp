#include "../Game/stdafx.h"

Player::Player(PlayerInfo* pPlayerInfo) : m_PlayerInfo(pPlayerInfo)
{

}

Player::~Player()
{

}

// 获取玩家id
std::string Player::GetUserId()
{
	if (!m_PlayerInfo)
	{
		return "";
	}

	return m_PlayerInfo->m_userId;
}

// 获取玩家信息
PlayerInfo* Player::GetPlayerInfo()
{
	return m_PlayerInfo;
}