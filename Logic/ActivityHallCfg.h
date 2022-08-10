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
	int breakdown;
	ActivityList(): id(0), type(0), sid(0), activityBreakdown(0), breakdown(0){}
	~ActivityList(){}
	void AddBeginTime(int tm)
	{
		beginTime.push_back(tm);
	}
	void AddEndTime(int tm)
	{
		endTime.push_back(tm);
	}
	void AddSolidTime(int tm)
	{
		solidTime.push_back(tm);
	}
	void AddOpenServerTime(int tm)
	{
		openServerTime.push_back(tm);
	}
	bool operator < (const ActivityList& other) const
	{
		if (id != other.id) { return id < other.id; }
		return false;
	}
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
	BrushMonsterCfg() : id(0), sid(0), mid(0), count(0), delayTime(0), refreshTime(0), x(0), y(0) {}
	~BrushMonsterCfg() {}
};

struct ActivityBreakdown
{
	int id;
	int brushMonsterCfg;
	IntVector beginTime;
	IntVector endTime;
	ActivityBreakdown() : id(0), brushMonsterCfg(0){}
	~ActivityBreakdown() {}
	void AddBeginTime(int tm)
	{
		beginTime.push_back(tm);
	}
	void AddEndTime(int tm)
	{
		endTime.push_back(tm);
	}
};

class ActivityHallCfg
{
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
	CfgMap<ActivityBreakdown>	m_ActivityBreakdownCfgMap;
};