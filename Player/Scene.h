#pragma once

class Scene
{
public:
	Scene();
	virtual ~Scene();

public:
	void Init();

public:
	void MessageDispatch(MsgCmd cmd, PlayerInfo* playerInfo);
	void SetPlayerPrepClient(PlayerPrepClient* playerPrepClient);

public:
	PlayerCenterClient& GetPlayerCenterClient();
	PlayerPrepClient* GetPlayerPrepClient();

private:
	PlayerPrepClient*		m_PlayerPrepClient;		// 玩家预处理
	PlayerCenterClient		m_PlayerCenterClient;	// 玩家中心
};
