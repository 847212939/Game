#include "../Game/stdafx.h"

PlayerClient::PlayerClient(const unsigned int& index, const uint64_t& userId) :
	Player(index, userId),
	m_MoveSys(this)
{

}

PlayerClient::~PlayerClient()
{

}

MoveSys& PlayerClient::GetMoveSys()
{ 
	return m_MoveSys; 
}
