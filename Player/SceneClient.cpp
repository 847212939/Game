#include "../Game/stdafx.h"

SceneClient::SceneClient(PlayerPrepClient* ppc) :
	Scene(),
	m_PlayerCenterClient(ppc), 
	m_ActivityHallSys(ppc)
{

}

SceneClient::~SceneClient()
{

}

PlayerCenterClient* SceneClient::GetPlayerCenterClient()
{
	return &m_PlayerCenterClient;
}

ActivityHallSys& SceneClient::GetActivityHallSys()
{
	return m_ActivityHallSys;
}