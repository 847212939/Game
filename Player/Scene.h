#pragma once

class Scene
{
public:
	Scene();
	virtual ~Scene();

public:
	void Init();
	void MessageDispatch(MsgCmd cmd, PlayerInfo* playerInfo);

public:
	PlayerCenterClient* GetPlayerCenterClient();

private:
	PlayerCenterClient	m_PlayerCenterClient;	// Íæ¼ÒÖÐÐÄ
};
