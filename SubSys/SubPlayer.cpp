#include "../Game/stdafx.h"

SubPlayer::SubPlayer(const unsigned int& index, const TCPSocketInfo* pSockInfo, const uint64_t& userId, SubPlayerPreproces* pp) :
	Player(index, pSockInfo, userId, pp),
	m_MoveSys(this)
{

}

SubPlayer::~SubPlayer()
{

}
