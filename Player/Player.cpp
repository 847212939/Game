#include "../Game/stdafx.h"

PlayerInfo::PlayerInfo() : pData(nullptr), pMsg(nullptr), pTcpSockInfo(nullptr), uSrverType(SocketType::SOCKET_TYPE_TCP)
{

}

PlayerInfo::~PlayerInfo()
{

}

Player::Player(TCPSocketInfo* sockInfo) : m_userId(0), m_TCPSocketInfo(sockInfo)
{

}

Player::~Player()
{

}

// ��ȡ���id
Player::UserId Player::GetUserId()
{
	return m_userId;
}

// ��ȡ���������Ϣ
TCPSocketInfo* Player::GetTCPSocketInfo()
{
	if (!m_TCPSocketInfo)
	{
		return nullptr;
	}
	return m_TCPSocketInfo;
}