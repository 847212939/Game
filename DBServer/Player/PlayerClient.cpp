#include "../stdafx.h"

PlayerClient::PlayerClient(const unsigned int& index) :
	Player(index),
	m_MoveSys(this),
	m_SkillLvSys(this)
{

}

PlayerClient::~PlayerClient()
{

}