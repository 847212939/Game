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

private:
	SceneClient	m_SceneClient;

public:
	GetClass(LoginSys, LoginSys)
};
