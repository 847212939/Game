#pragma once

struct CActivityList
{
	int id;
	int type;
	int sid;
	IntVector beginTime;
	IntVector endTime;
	IntVector solidTime;
	IntVector openServerTime;
	int activityBreakdown;
	CActivityList();
	~CActivityList();
	void AddBeginTime(int tm);
	void AddEndTime(int tm);
	void AddSolidTime(int tm);
	void AddOpenServerTime(int tm);
	bool operator < (const CActivityList& other) const;
};

struct CActivityBreakdown
{
	int id;
	int dayBreakdown;
	int hourBreakdown;
	CfgMap<int> dayBreakdownList;
	CfgMap<int> hourBreakdownList;
	CActivityBreakdown();
	~CActivityBreakdown();
	void AddDayBreakdownList(int idex, int tm);
	void AddHourBreakdownList(int idex, int tm);

	CActivityBreakdown(int nid);
	int GetDayBrushMonsterCfgid(int& index) const;
	int GetHourBrushMonsterCfgid(int& index) const;
	int GetDayBrushMonsterCfg(int index) const;
	int GetHourBrushMonsterCfg(int index) const;
	bool operator < (const CActivityBreakdown& other) const;
};

struct CBrushMonsterCfg
{
	int id;
	int sid;
	int mid;
	int count;
	int delayTime;
	int refreshTime;
	int x;
	int y;
	CBrushMonsterCfg();
	~CBrushMonsterCfg();
};

class ActivityHallCfg
{
public:
	CfgSet<CActivityList>& GetActivityListCfgSet();
	const CActivityBreakdown* GetActivityBreakdown(int id);
	CfgVector<CBrushMonsterCfg>* GetBrushMonsterCfg(int id);

public:
	ActivityHallCfg() {}
	~ActivityHallCfg() {}

public:
	void ReadActivityList(CActivityList* config);
	void ReadBrushMonsterCfg(CBrushMonsterCfg* config);
	void ReadActivityBreakdownCfg(CActivityBreakdown* config);

private:
	CfgSet<CActivityList>		m_ActivityListCfgSet;
	CfgMap<CBrushMonsterCfg>		m_BrushMonsterCfgMap;
	CfgSet<CActivityBreakdown>	m_ActivityBreakdownCfgSet;
};