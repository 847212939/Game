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
	ActivityList(): id(0), type(0), sid(0), activityBreakdown(0){}
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

struct ActivityBreakdown
{
	int id;
	int dayBreakdown;
	int hourBreakdown;
	CfgMap<int> dayBreakdownList;
	CfgMap<int> hourBreakdownList;
	ActivityBreakdown() : id(0), dayBreakdown(0), hourBreakdown(0) {}
	~ActivityBreakdown() {}
	void AddDayBreakdownList(int idex, int tm)
	{
		CfgMap<int>::iterator it = dayBreakdownList.find(idex);
		if (it == dayBreakdownList.end())
		{
			CfgVector<int> tmp;
			tmp.push_back(tm);
			dayBreakdownList.insert({ idex , tmp });
		}
		else
		{
			it->second.push_back(tm);
		}
	}
	void AddHourBreakdownList(int idex, int tm)
	{
		CfgMap<int>::iterator it = hourBreakdownList.find(idex);
		if (it == hourBreakdownList.end())
		{
			CfgVector<int> tmp;
			tmp.push_back(tm);
			hourBreakdownList.insert({ idex , tmp });
		}
		else
		{
			it->second.push_back(tm);
		}
	}

	ActivityBreakdown(int nid) : id(nid), dayBreakdown(0), hourBreakdown(0) {}
	int GetDayBrushMonsterCfgid(int& index) const
	{
		int day = Util::GetServiceDays();
		for (auto& m : dayBreakdownList)
		{
			if (m.second.size() != 3)
			{
				continue;
			}
			if (m.second[2] <= 0)
			{
				continue;
			}
			if (day >= m.second[0] && day <= m.second[1])
			{
				index = m.first;
				return m.second[2];
			}
			if (m.second[0] == m.second[1] && m.second[0] == 0 && m.second[1] == 0)
			{
				index = m.first;
				return m.second[2];
			}
		}

		return 0;
	}
	int GetHourBrushMonsterCfgid(int& index) const
	{
		uint64_t sysSecond = Util::GetSysSecond();
		for (auto& m : hourBreakdownList)
		{
			if (m.second.size() != 7)
			{
				continue;
			}
			if (m.second[6] <= 0)
			{
				continue;
			}

			uint64_t beginTime = Util::GetCfgSecond(m.second);
			uint64_t endTime = Util::GetCfgSecondEnd(m.second);
			
			if (sysSecond >= beginTime && sysSecond <= endTime)
			{
				index = m.first;
				return m.second[6];
			}
			if (beginTime == endTime && beginTime == 0 && endTime == 0)
			{
				index = m.first;
				return m.second[6];
			}
		}

		return 0;
	}
	int GetDayBrushMonsterCfg(int index) const
	{
		CfgMap<int>::const_iterator it = dayBreakdownList.find(index);
		if (it == dayBreakdownList.end())
		{
			return 0;
		}
		if (it->second.size() != 3)
		{
			return 0;
		}
		return it->second[2];
	}
	int GetHourBrushMonsterCfg(int index) const
	{
		CfgMap<int>::const_iterator it = hourBreakdownList.find(index);
		if (it == hourBreakdownList.end())
		{
			return 0;
		}
		if (it->second.size() != 7)
		{
			return 0;
		}
		return it->second[6];
	}
	bool operator < (const ActivityBreakdown& other) const
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