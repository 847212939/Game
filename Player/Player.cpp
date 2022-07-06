#include "../Game/stdafx.h"

Player::Player(const TCPSocketInfo* pSockInfo, std::string& userId) : 
	m_pTcpSockInfo(pSockInfo),
	m_userId(userId)
{

}

Player::~Player()
{

}

// �������ݿ�
void Player::LoadMysql()
{

}

bool Player::EnterScene()
{
	return true;
}

// ������Ϸ
void Player::EnterGame()
{

}

// ��ȡ���id
std::string Player::GetUserId() const
{
	return m_userId;
}

// ��ȡ�����Ϣ
const TCPSocketInfo* Player::GetTCPSocketInfo()
{
	return m_pTcpSockInfo;
}