#include "../Game/stdafx.h"

SubPlayer::SubPlayer(const unsigned int& index, const TCPSocketInfo* pSockInfo, const uint64_t& userId) :
	Player(index, pSockInfo, userId),
	m_MoveSys(this)
{

}

SubPlayer::~SubPlayer()
{

}
