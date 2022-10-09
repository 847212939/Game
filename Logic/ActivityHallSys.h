#pragma once

enum class ActType
{
	at_section_open		= 1,		// ʱ������
	at_always_open		= 2,		// ȫ�쿪��
	at_service_open		= 3,		// �����
	at_timed_open		= 4,		// �̶�ʱ�俪��
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
	int GetBrushMonsterId(const CActivityBreakdown* pConfig, std::pair<int, int>& pr);
	int GetPreBrushMonsterId(const CActivityBreakdown* pConfig, int type, int index);

public:
	// ��ȡˢ������
	VectorTemplate<CBrushMonsterCfg>* GetBrushMonsterCfg(CActivityList* cfg, int& bmid);

public:
	// �����ϴλ������
	void ClearBrushMonsterCfgVec(const CActivityBreakdown* pConfig, std::pair<int, int>& pr);

public:
	// ˢ��
	bool InitMonster(CBrushMonsterCfg& cfg);
	bool CreateMonster(std::vector<Animal*>* pValue, CBrushMonsterCfg& cfg);

public:
	// ��ӳ�������
	void DelRefMonster(int sid, RefMonsterKey& key);
	void AddRefMonster(int sid, RefMonsterKey& key, std::vector<Animal*>& value);
	std::vector<Animal*>* GetRefMonster(int sid, RefMonsterKey& key);

public:
	// ͨ�ó���������˳�
	bool Enter(CActivityList* cfg, int& bmid);
	bool Exit(CActivityList* cfg, const int& bmid);

private:
	// ��ص�
	bool ActiveCallBackFun(ActType type, CActivityList* cfg);
	void AddActiveCallback(ActType type, std::function<bool(CActivityList*)>&& fun);
	bool ActiveExitCallBackFun(ActType type, CActivityList* cfg);
	void AddActiveExitCallback(ActType type, std::function<bool(CActivityList*)>&& fun);
	bool ActiveEnterCallBackFun(ActType type, CActivityList* cfg);
	void AddActiveEnterCallback(ActType type, std::function<bool(CActivityList*)>&& fun);

private:
	// ������ж�
	bool AtSectionOpen(CActivityList* cfg);
	bool AtAlwaysOpen(CActivityList* cfg);
	bool AtServiceOpen(CActivityList* cfg);
	bool AtTimedOpen(CActivityList* cfg);

private:
	// ��ʱ��
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