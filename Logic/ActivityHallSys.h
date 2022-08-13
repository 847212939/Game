#pragma once

enum class ActType
{
	at_section_open		= 1,			// ʱ������
	at_always_open		= 2,			// ȫ�쿪��
	at_service_open		= 3,			// �����
	at_timed_open		= 4,			// �̶�ʱ�俪��
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
	// ������ж�
	bool GetActiveOpen(int id);

public:
	// ��ȡˢ��id
	int GetBrushMonsterId(const ActivityBreakdown* pConfig, std::pair<int, int>& pr);
	int GetPreBrushMonsterId(const ActivityBreakdown* pConfig, int type, int index);

public:
	// ��ȡˢ������
	CfgVector<BrushMonsterCfg>* GetBrushMonsterCfgVec(ActivityList* cfg);

public:
	// �����ϴλ������
	void ClearBrushMonsterCfgVec(const ActivityBreakdown* pConfig, std::pair<int, int>& pr);

public:
	// ˢ��
	bool InitMonster(BrushMonsterCfg& cfg);
	bool CreateMonster(std::vector<Animal*>* pValue, BrushMonsterCfg& cfg);

public:
	// ��ӳ�������
	void DelRefMonsterVec(int sid, RefMonsterKey& key);
	void AddRefMonsterVec(int sid, RefMonsterKey& key, std::vector<Animal*>& value);
	std::vector<Animal*>* GetRefMonsterVec(int sid, RefMonsterKey& key);

private:
	// ��ص�
	bool ActiveCallBackFun(ActType type, ActivityList* cfg);
	void AddActiveCallback(ActType type, std::function<bool(ActivityList*)>&& fun);
	bool ActiveExitCallBackFun(ActType type, ActivityList* cfg);
	void AddActiveExitCallback(ActType type, std::function<bool(ActivityList*)>&& fun);
	bool ActiveEnterCallBackFun(ActType type, ActivityList* cfg);
	void AddActiveEnterCallback(ActType type, std::function<bool(ActivityList*)>&& fun);

private:
	// ������ж�
	bool AtSectionOpen(ActivityList* cfg);
	bool AtAlwaysOpen(ActivityList* cfg);
	bool AtServiceOpen(ActivityList* cfg);
	bool AtTimedOpen(ActivityList* cfg);

private:
	// ��ʱ��
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