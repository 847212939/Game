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
	int brushMonsterCfg;
	ActivityList(): id(0), type(0), sid(0), brushMonsterCfg(0) {}
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
	int echelon;
	int sid;
	int mid;
	int count;
	int delayTime;
	int refreshTime;
	int x;
	int y;
	BrushMonsterCfg() : id(0), echelon(0), sid(0), mid(0), count(0), delayTime(0), refreshTime(0), x(0), y(0) {}
	~BrushMonsterCfg() {}
};

struct MiningCfg
{
	int echelon;
	IntVector beginTime;
	IntVector endTime;
	MiningCfg() : echelon(0) {}
	~MiningCfg() {}
	void AddBeginTime(int tm)
	{
		beginTime.push_back(tm);
	}
	void AddEndTime(int tm)
	{
		endTime.push_back(tm);
	}
	bool operator < (const MiningCfg& other) const
	{
		if (echelon != other.echelon) { return echelon < other.echelon; }
		return false;
	}
};

using BrushMonsterCfgMap = std::map<int, CfgVector<BrushMonsterCfg>>;

class ActivityHallCfg
{
public:
	ActivityHallCfg() {}
	~ActivityHallCfg() {}

public:
	void ReadMiningCfg(MiningCfg* config);
	void ReadActivityList(ActivityList* config);
	void ReadBrushMonsterCfg(BrushMonsterCfg* config);

private:
	CfgSet<MiningCfg>		m_MiningCfgSet;
	CfgSet<ActivityList>	m_ActivityListCfgSet;
	BrushMonsterCfgMap		m_BrushMonsterCfgMap;
};