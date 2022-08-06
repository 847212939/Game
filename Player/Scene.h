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
	void SetSubPlayerPreproces(SubPlayerPreproces* pSubPlayerPreproces);

public:
	PlayerCenterClient& GetPlayerCenter();
	SubPlayerPreproces* GetPlayerPreproces();

private:
	PlayerCenterClient		m_PlayerCenterClient;			// 玩家中心
	SubPlayerPreproces*		m_SubPlayerPreproces;		// 玩家预处理
};
