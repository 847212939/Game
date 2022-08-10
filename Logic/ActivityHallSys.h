#pragma once

enum class ActType
{
	at_section_open = 1,		// 时间区间
	at_always_open = 2,			// 全天开启


};

using ActivityFunMap = std::map<ActType, std::function<void(ActivityList*)>>;

class ActivityHallSys
{
public:
	ActivityHallSys();
	~ActivityHallSys();

public:
	void AddActiveCallback(ActType type, std::function<void(ActivityList*)>&& fun);
	void ActiveCallBackFun(ActType type, ActivityList* playerInfo);

private:

};