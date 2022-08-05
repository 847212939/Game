#pragma once

class PlayerPrepClient : public PlayerPrep
{
public:
	PlayerPrepClient();
	virtual~PlayerPrepClient();

protected:
	PlayerPrepClient(const PlayerPrepClient& my);
	PlayerPrepClient& operator=(const PlayerPrepClient& my);

public:
	SceneClient* GetSceneClient();
	LoginSys& GetLoginSys();

private:
	SceneClient		m_SceneClient;
	LoginSys		m_LoginSys;
};
