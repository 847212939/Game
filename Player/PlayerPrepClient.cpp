#include "../Game/stdafx.h"

PlayerPrepClient::PlayerPrepClient() : 
	PlayerPrep(), 
	m_SceneClient(this),
	m_LoginSys(this)
{
	InitMysqlTable();
}

PlayerPrepClient::~PlayerPrepClient()
{
}

SceneClient* PlayerPrepClient::GetSceneClient()
{
	return &m_SceneClient;
}