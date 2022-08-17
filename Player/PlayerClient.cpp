#include "../Game/stdafx.h"

PlayerClient::PlayerClient(const unsigned int& index) :
	Player(index),
	m_MoveSys(this)
{

}

PlayerClient::~PlayerClient()
{

}