#pragma once

class SceneClient : public Scene
{
public:
	SceneClient(PlayerPrepClient* ppc);
	virtual~SceneClient();

protected:
	SceneClient(const SceneClient& my);
	SceneClient& operator=(const SceneClient& my);

public:
	// 获取玩家中心
	PlayerCenterClient* GetPlayerCenterClient();

private:
	PlayerCenterClient	m_PlayerCenterClient;
};
