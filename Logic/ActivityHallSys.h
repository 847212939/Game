#pragma once

enum class ActType
{
	at_section_open		= 1,			// ʱ������
	at_always_open		= 2,			// ȫ�쿪��
	at_service_open		= 3,			// �����
	at_timed_open		= 4,			// �̶�ʱ�俪��
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
	// ������ж�
	bool AtSectionOpen(ActivityList* cfg);
	bool AtAlwaysOpen(ActivityList* cfg);
	bool AtServiceOpen(ActivityList* cfg);
	bool AtTimedOpen(ActivityList* cfg);

private:
	void TimerCallback();

private:
	ActivityFunMap m_ActivityFunMap;
};