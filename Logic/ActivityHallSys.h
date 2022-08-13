#pragma once

enum class ActType
{
	at_section_open		= 1,			// 时间区间
	at_always_open		= 2,			// 全天开启
	at_service_open		= 3,			// 开服活动
	at_timed_open		= 4,			// 固定时间开启
};

struct ActtiveOpen
{
	int id;
	bool open;
	ActtiveOpen() : id(0), open(false){}
	ActtiveOpen(int nId, bool isOpen) : id(nId), open(isOpen) {}
	~ActtiveOpen(){}
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
	int GetBrushMonsterId(const ActivityBreakdown* pConfig, std::pair<int, int>& pr);
	int GetPreBrushMonsterId(const ActivityBreakdown* pConfig, int type, int index);

public:
	// 获取刷怪配置
	CfgVector<BrushMonsterCfg>* GetBrushMonsterCfgVec(ActivityList* cfg);

public:
	// 清理上次活动残留怪
	void ClearBrushMonsterCfgVec(const ActivityBreakdown* pConfig, std::pair<int, int>& pr);

public:
	// 刷怪
	bool InitMonster(BrushMonsterCfg& cfg);
	bool CreateMonster(std::vector<Animal*>* pValue, BrushMonsterCfg& cfg);

public:
	// 添加场景怪物
	void DelRefMonsterVec(int sid, RefMonsterKey& key);
	void AddRefMonsterVec(int sid, RefMonsterKey& key, std::vector<Animal*>& value);
	std::vector<Animal*>* GetRefMonsterVec(int sid, RefMonsterKey& key);

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
	ActiveSection	m_ActiveSection;
	ActiveAlways	m_ActiveAlways;
	ActiveService	m_ActiveService;
	ActiveTime		m_ActiveTime;
	ActtiveOpenMap	m_ActtiveOpenMap;
	MonsterMap		m_MonsterMap;
};