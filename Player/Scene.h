#pragma once

class Scene
{
public:
	Scene();
	virtual ~Scene();

public:
	void Init();

public:
	void DispatchMessage(MsgCmd cmd, PlayerInfo* pPlayerInfo);
	void SetSubPlayerPreproces(PlayerPrepClient* pSubPlayerPreproces);

public:
	PlayerCenterClient& GetPlayerCenter();
	PlayerPrepClient* GetPlayerPreproces();

private:
	PlayerCenterClient		m_PlayerCenterClient;			// 玩家中心
	PlayerPrepClient*		m_PlayerPrepClient;		// 玩家预处理
};
