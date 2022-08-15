#pragma once

class ActivityHallSys
{
public:
	ActivityHallSys(PlayerPrepClient* ppc);
	virtual~ActivityHallSys();

public:
	// 活动开启判断
	bool GetActiveOpen(int id);

public:
	// 获取刷怪id
	int GetBrushMonsterId(const ActivityBreakdown* pConfig, std::pair<int, int>& pr);
	int GetPreBrushMonsterId(const ActivityBreakdown* pConfig, int type, int index);

public:
	// 获取刷怪配置
	CfgVector<BrushMonsterCfg>* GetBrushMonsterCfg(ActivityList* cfg, int& bmid);

public:
	// 清理上次活动残留怪
	void ClearBrushMonsterCfgVec(const ActivityBreakdown* pConfig, std::pair<int, int>& pr);

public:
	// 刷怪
	bool InitMonster(BrushMonsterCfg& cfg);
	bool CreateMonster(std::vector<Animal*>* pValue, BrushMonsterCfg& cfg);

public:
	// 添加场景怪物
	void DelRefMonster(int sid, RefMonsterKey& key);
	void AddRefMonster(int sid, RefMonsterKey& key, std::vector<Animal*>& value);
	std::vector<Animal*>* GetRefMonster(int sid, RefMonsterKey& key);

public:
	// 通用场景进入和退出
	bool Enter(ActivityList* cfg, int& bmid);
	bool Exit(ActivityList* cfg, const int& bmid);

private:
	// 活动回调
	bool ActiveCallBackFun(ActType type, ActivityList* cfg);
	void AddActiveCallback(ActType type, std::function<bool(ActivityList*)>&& fun);
	bool ActiveExitCallBackFun(ActType type, ActivityList* cfg);
	void AddActiveExitCallback(ActType type, std::function<bool(ActivityList*)>&& fun);
	bool ActiveEnterCallBackFun(ActType type, ActivityList* cfg);
	void AddActiveEnterCallback(ActType type, std::function<bool(ActivityList*)>&& fun);

private:
	// 活动开启判断
	bool AtSectionOpen(ActivityList* cfg);
	bool AtAlwaysOpen(ActivityList* cfg);
	bool AtServiceOpen(ActivityList* cfg);
	bool AtTimedOpen(ActivityList* cfg);

private:
	// 定时器
	void TimerCallback();

private:
	ActivityFunMap	m_ActivityFunMap;
	ActivityFunMap	m_ActivityEnterFunMap;
	ActivityFunMap	m_ActivityExitFunMap;
	ActtiveOpenMap	m_ActtiveOpenMap;
	MonsterMap		m_MonsterMap;

public:
	GetClass(ActiveSection, ActiveSection)
	GetClass(ActiveAlways, ActiveAlways)
	GetClass(ActiveService, ActiveService)
	GetClass(ActiveTime, ActiveTime)
};