#include "../Game/stdafx.h"

PlayerPrepClient::PlayerPrepClient() : 
	PlayerPrep(), 
	m_SceneClient(this),
	m_LoginSys(this)
{
	RegisterCreat(this, "move");
	RegisterCreatS(this, "useraccount");
	RegisterCreatS(this, "skilllevel");
}

PlayerPrepClient::~PlayerPrepClient()
{
}

SceneClient* PlayerPrepClient::GetSceneClient()
{
	return &m_SceneClient;
}