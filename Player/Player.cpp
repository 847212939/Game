#include "../Game/stdafx.h"

Player::Player(PlayerInfo* pPlayerInfo) : m_PlayerInfo(pPlayerInfo)
{

}

Player::~Player()
{

}

// ��ȡ���id
std::string Player::GetUserId()
{
	if (!m_PlayerInfo)
	{
		return "";
	}

	return m_PlayerInfo->m_userId;
}

// ��ȡ�����Ϣ
PlayerInfo* Player::GetPlayerInfo()
{
	return m_PlayerInfo;
}