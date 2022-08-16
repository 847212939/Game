#include "../Game/stdafx.h"

PlayerClient::PlayerClient(const unsigned int& index) :
	Player(index),
	m_MoveSys(this),
	m_SkillSys(this)
{

}

PlayerClient::~PlayerClient()
{

}