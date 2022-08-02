#include "../Game/stdafx.h"

PlayerPrepClient::PlayerPrepClient() : 
	PlayerPrep(), 
	m_SceneClient(this),
	m_LoginSys(this)
{
	RegisterCreat(this, "move");
	RegisterCreatS(this, "useraccount");
}

PlayerPrepClient::~PlayerPrepClient()
{
}

SceneClient* PlayerPrepClient::GetSceneClient()
{
	return &m_SceneClient;
}


LoginSys& PlayerPrepClient::GetLoginSys()
{
	return m_LoginSys;
}