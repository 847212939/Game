#include "../Game/stdafx.h"

PlayerClient::PlayerClient(const unsigned int& index, const TCPSocketInfo* pSockInfo, const uint64_t& userId) :
	Player(index, pSockInfo, userId),
	m_MoveSys(this)
{

}

PlayerClient::~PlayerClient()
{

}
