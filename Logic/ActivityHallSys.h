#pragma once

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
	CfgVector<BrushMonsterCfg>* GetBrushMonsterCfg(ActivityList* cfg, int& bmid);

public:
	// �����ϴλ������
	void ClearBrushMonsterCfgVec(const ActivityBreakdown* pConfig, std::pair<int, int>& pr);

public:
	// ˢ��
	bool InitMonster(BrushMonsterCfg& cfg);
	bool CreateMonster(std::vector<Animal*>* pValue, BrushMonsterCfg& cfg);

public:
	// ��ӳ�������
	void DelRefMonster(int sid, RefMonsterKey& key);
	void AddRefMonster(int sid, RefMonsterKey& key, std::vector<Animal*>& value);
	std::vector<Animal*>* GetRefMonster(int sid, RefMonsterKey& key);

public:
	// ͨ�ó���������˳�
	bool Enter(ActivityList* cfg, int& bmid);
	bool Exit(ActivityList* cfg, const int& bmid);

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
	ActtiveOpenMap	m_ActtiveOpenMap;
	MonsterMap		m_MonsterMap;

public:
	GetClass(ActiveSection, ActiveSection)
	GetClass(ActiveAlways, ActiveAlways)
	GetClass(ActiveService, ActiveService)
	GetClass(ActiveTime, ActiveTime)
};