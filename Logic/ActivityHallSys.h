#pragma once

enum class ActType
{
	at_section_open = 1,		// ʱ������
	at_always_open = 2,			// ȫ�쿪��


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