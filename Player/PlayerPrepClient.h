#pragma once

class PlayerPrepClient : public PlayerPrep
{
public:
	PlayerPrepClient();
	virtual~PlayerPrepClient();

public:
	SceneClient* GetSceneClient();
	LoginSys& GetLoginSys();

private:
	SceneClient		m_SceneClient;
	LoginSys		m_LoginSys;
};
