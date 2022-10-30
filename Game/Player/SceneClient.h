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
	// 注册场景相关的数据库回调
	void RegisterGlobalCallBack();
	// 获取玩家中心
	PlayerCenterClient* GetPlayerCenterClient();

private:
	PlayerCenterClient	m_PlayerCenterClient;

public:
	GetClass(ActivityHallSys, ActivityHallSys)		// 活动系统
	GetClass(SkillSys, SkillSys)					// 技能系统 
};
