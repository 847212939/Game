#include "../Game/stdafx.h"

SceneClient::SceneClient(PlayerPrepClient* ppc) :
	Scene(),
	m_PlayerCenterClient(ppc), 
	m_ActivityHallSys(ppc),
	m_SkillSys(ppc)
{

}

SceneClient::~SceneClient()
{

}

PlayerCenterClient* SceneClient::GetPlayerCenterClient()
{
	return &m_PlayerCenterClient;
}