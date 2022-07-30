#include "../Game/stdafx.h"

PlayerClient::PlayerClient(const unsigned int& index, const TCPSocketInfo* pSockInfo, const uint64_t& userId, PlayerPrepClient* pp) :
	Player(index, pSockInfo, userId, pp),
	m_MoveSys(this)
{

}

PlayerClient::~PlayerClient()
{

}
