#pragma once

class SceneClient : public Scene
{
public:
	SceneClient(PlayerPrepClient* ppc);
	virtual~SceneClient();

public:
	PlayerCenterClient* GetPlayerCenterClient();

private:
	PlayerCenterClient	m_PlayerCenterClient;
};
