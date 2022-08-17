#include "../Game/stdafx.h"

PlayerClient::PlayerClient(const unsigned int& index) :
	Player(index),
	m_MoveSys(this),
	m_SkillLevelSys(this)
{

}

PlayerClient::~PlayerClient()
{

}