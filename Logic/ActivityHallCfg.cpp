#include "../Game/stdafx.h"

BrushMonsterCfg::BrushMonsterCfg() : id(0), sid(0), mid(0), count(0), delayTime(0), refreshTime(0), x(0), y(0) 
{
}

BrushMonsterCfg::~BrushMonsterCfg()
{
}

ActivityList::ActivityList() : id(0), type(0), sid(0), activityBreakdown(0) 
{
}

ActivityList::~ActivityList()
{
}

void ActivityList::AddBeginTime(int tm)
{
	beginTime.push_back(tm);
}

void ActivityList::AddEndTime(int tm)
{
	endTime.push_back(tm);
}

void ActivityList::AddSolidTime(int tm)
{
	solidTime.push_back(tm);
}

void ActivityList::AddOpenServerTime(int tm)
{
	openServerTime.push_back(tm);
}

bool ActivityList::operator < (const ActivityList& other) const
{
	if (id != other.id) { return id < other.id; }
	return false;
}

ActivityBreakdown::ActivityBreakdown() : id(0), dayBreakdown(0), hourBreakdown(0) 
{
}

ActivityBreakdown::~ActivityBreakdown() 
{
}

void ActivityBreakdown::AddDayBreakdownList(int idex, int tm)
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

void ActivityBreakdown::AddHourBreakdownList(int idex, int tm)
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

ActivityBreakdown::ActivityBreakdown(int nid) : id(nid), dayBreakdown(0), hourBreakdown(0) 
{
}

int ActivityBreakdown::GetDayBrushMonsterCfgid(int& index) const
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

int ActivityBreakdown::GetHourBrushMonsterCfgid(int& index) const
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

int ActivityBreakdown::GetDayBrushMonsterCfg(int index) const
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

int ActivityBreakdown::GetHourBrushMonsterCfg(int index) const
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

bool ActivityBreakdown::operator < (const ActivityBreakdown& other) const
{
	if (id != other.id) { return id < other.id; }
	return false;
}

CfgSet<ActivityList>& ActivityHallCfg::GetActivityListCfgSet()
{
	return m_ActivityListCfgSet;
}

const ActivityBreakdown* ActivityHallCfg::GetActivityBreakdown(int id)
{
	CfgSet<ActivityBreakdown>::iterator it = m_ActivityBreakdownCfgSet.find(id);
	if (it == m_ActivityBreakdownCfgSet.end())
	{
		return nullptr;
	}
	return &(*it);
}

CfgVector<BrushMonsterCfg>* ActivityHallCfg::GetBrushMonsterCfg(int id)
{
	CfgMap<BrushMonsterCfg>::iterator it = m_BrushMonsterCfgMap.find(id);
	if (it == m_BrushMonsterCfgMap.end())
	{
		return nullptr;
	}

	return &it->second;
}

void ActivityHallCfg::ReadActivityBreakdownCfg(ActivityBreakdown* config)
{
	m_ActivityBreakdownCfgSet.insert(*config);
}

void ActivityHallCfg::ReadActivityList(ActivityList* config)
{
	m_ActivityListCfgSet.insert(*config);
}

void ActivityHallCfg::ReadBrushMonsterCfg(BrushMonsterCfg* config)
{
	CfgMap<BrushMonsterCfg>::iterator it = m_BrushMonsterCfgMap.find(config->id);
	if (it == m_BrushMonsterCfgMap.end())
	{
		CfgVector<BrushMonsterCfg> tmp;
		tmp.push_back(*config);
		m_BrushMonsterCfgMap.insert({ config->id, tmp });
	}
	else
	{
		it->second.push_back(*config);
	}
}