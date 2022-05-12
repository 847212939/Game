#pragma once

class Scene
{
public:
	Scene(PlayerPreproces* pPlayerPreproces);
	~Scene();

public:
	// 分发消息
	void DispatchMessage(MsgCmd cmd, PlayerInfo* pPlayerInfo);
	// 创建角色
	bool CreatePlayr(PlayerInfo* pPlayerInfo);
	// 获取玩家预处理
	PlayerPreproces* GetPlayerPreproces();

private:
	// 初始化回调函数
	void InitCallBackFun();

private:
	// 玩家预处理
	PlayerPreproces* m_pPlayerPreproces;
	// 玩家中心
	PlayerCenter     m_PlayerCenter;
};
