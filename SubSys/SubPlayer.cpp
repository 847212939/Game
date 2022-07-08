#include "../Game/stdafx.h"

SubPlayer::SubPlayer(const unsigned int& index, const TCPSocketInfo* pSockInfo, const long long& userId) :
	Player(index, pSockInfo, userId),
	m_MoveSys(this)
{

}

SubPlayer::~SubPlayer()
{

}
