#pragma once

enum class ActType
{
	at_section_open		= 1,			// 时间区间
	at_always_open		= 2,			// 全天开启
	at_service_open		= 3,			// 开服活动
	at_timed_open		= 4,			// 固定时间开启
};

class ActivityHallSys
{
public:
	ActivityHallSys(PlayerPrepClient* ppc);
	virtual~ActivityHallSys();

public:
	bool ActiveCallBackFun(ActType type, ActivityList* cfg);
	void AddActiveCallback(ActType type, std::function<bool(ActivityList*)>&& fun);

private:
	// 活动开启判断
	bool AtSectionOpen(ActivityList* cfg);
	bool AtAlwaysOpen(ActivityList* cfg);
	bool AtServiceOpen(ActivityList* cfg);
	bool AtTimedOpen(ActivityList* cfg);

private:
	void TimerCallback();

private:
	ActivityFunMap m_ActivityFunMap;
};