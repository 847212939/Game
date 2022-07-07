#pragma once

class Scene
{
public:
	Scene(SubPlayerPreproces* pSubPlayerPreproces);
	virtual ~Scene();

public:
	void DispatchMessage(MsgCmd cmd, PlayerInfo* pPlayerInfo);

public:
	SubPlayerCenter& GetPlayerCenter();
	SubPlayerPreproces* GetPlayerPreproces();

private:
	SubPlayerPreproces*		m_SubPlayerPreproces;		// 玩家预处理
	SubPlayerCenter			m_SubPlayerCenter;			// 玩家中心
};
