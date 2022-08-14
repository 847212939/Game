#pragma once

struct ActivityList
{
	int id;
	int type;
	int sid;
	IntVector beginTime;
	IntVector endTime;
	IntVector solidTime;
	IntVector openServerTime;
	int activityBreakdown;
	ActivityList();
	~ActivityList();
	void AddBeginTime(int tm);
	void AddEndTime(int tm);
	void AddSolidTime(int tm);
	void AddOpenServerTime(int tm);
	bool operator < (const ActivityList& other) const;
};

struct ActivityBreakdown
{
	int id;
	int dayBreakdown;
	int hourBreakdown;
	CfgMap<int> dayBreakdownList;
	CfgMap<int> hourBreakdownList;
	ActivityBreakdown();
	~ActivityBreakdown();
	void AddDayBreakdownList(int idex, int tm);
	void AddHourBreakdownList(int idex, int tm);

	ActivityBreakdown(int nid);
	int GetDayBrushMonsterCfgid(int& index) const;
	int GetHourBrushMonsterCfgid(int& index) const;
	int GetDayBrushMonsterCfg(int index) const;
	int GetHourBrushMonsterCfg(int index) const;
	bool operator < (const ActivityBreakdown& other) const;
};

struct BrushMonsterCfg
{
	int id;
	int sid;
	int mid;
	int count;
	int delayTime;
	int refreshTime;
	int x;
	int y;
	BrushMonsterCfg();
	~BrushMonsterCfg();
};

class ActivityHallCfg
{
public:
	CfgSet<ActivityList>& GetActivityListCfgSet();
	const ActivityBreakdown* GetActivityBreakdown(int id);
	CfgVector<BrushMonsterCfg>* GetBrushMonsterCfg(int id);

public:
	ActivityHallCfg() {}
	~ActivityHallCfg() {}

public:
	void ReadActivityList(ActivityList* config);
	void ReadBrushMonsterCfg(BrushMonsterCfg* config);
	void ReadActivityBreakdownCfg(ActivityBreakdown* config);

private:
	CfgSet<ActivityList>		m_ActivityListCfgSet;
	CfgMap<BrushMonsterCfg>		m_BrushMonsterCfgMap;
	CfgSet<ActivityBreakdown>	m_ActivityBreakdownCfgSet;
};