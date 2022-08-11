#include "../Game/stdafx.h"

PlayerClient::PlayerClient(const unsigned int& index) :
	Player(index),
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
