#pragma once

enum class ActType
{
	at_section_open		= 1,		// 时间区间
	at_always_open		= 2,		// 全天开启
	at_service_open		= 3,		// 开服活动
	at_timed_open		= 4,		// 固定时间开启
};

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
	int GetBrushMonsterId(const CActivityBreakdown* pConfig, std::pair<int, int>& pr);
	int GetPreBrushMonsterId(const CActivityBreakdown* pConfig, int type, int index);

public:
	// 获取刷怪配置
	VectorTemplate<CBrushMonsterCfg>* GetBrushMonsterCfg(CActivityList* cfg, int& bmid);

public:
	// 清理上次活动残留怪
	void ClearBrushMonsterCfgVec(const CActivityBreakdown* pConfig, std::pair<int, int>& pr);

public:
	// 刷怪
	bool InitMonster(CBrushMonsterCfg& cfg);
	bool CreateMonster(std::vector<Animal*>* pValue, CBrushMonsterCfg& cfg);

public:
	// 添加场景怪物
	void DelRefMonster(int sid, RefMonsterKey& key);
	void AddRefMonster(int sid, RefMonsterKey& key, std::vector<Animal*>& value);
	std::vector<Animal*>* GetRefMonster(int sid, RefMonsterKey& key);

public:
	// 通用场景进入和退出
	bool Enter(CActivityList* cfg, int& bmid);
	bool Exit(CActivityList* cfg, const int& bmid);

private:
	// 活动回调
	bool ActiveCallBackFun(ActType type, CActivityList* cfg);
	void AddActiveCallback(ActType type, std::function<bool(CActivityList*)>&& fun);
	bool ActiveExitCallBackFun(ActType type, CActivityList* cfg);
	void AddActiveExitCallback(ActType type, std::function<bool(CActivityList*)>&& fun);
	bool ActiveEnterCallBackFun(ActType type, CActivityList* cfg);
	void AddActiveEnterCallback(ActType type, std::function<bool(CActivityList*)>&& fun);

private:
	// 活动开启判断
	bool AtSectionOpen(CActivityList* cfg);
	bool AtAlwaysOpen(CActivityList* cfg);
	bool AtServiceOpen(CActivityList* cfg);
	bool AtTimedOpen(CActivityList* cfg);

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