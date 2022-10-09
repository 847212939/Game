#include "../Game/stdafx.h"

CBrushMonsterCfg::CBrushMonsterCfg() : id(0), sid(0), mid(0), count(0), delayTime(0), refreshTime(0), x(0), y(0) 
{
}

CActivityBreakdown::CActivityBreakdown() : id(0), dayBreakdown(0), hourBreakdown(0)
{
}

CActivityBreakdown::CActivityBreakdown(int nid) : id(nid), dayBreakdown(0), hourBreakdown(0)
{
}

CActivityList::CActivityList() : id(0), type(0), sid(0), activityBreakdown(0)
{
}

CBrushMonsterCfg::~CBrushMonsterCfg()
{
}

CActivityList::~CActivityList()
{
}

CActivityBreakdown::~CActivityBreakdown()
{
}

void CActivityList::AddBeginTime(int tm)
{
	beginTime.push_back(tm);
}

void CActivityList::AddEndTime(int tm)
{
	endTime.push_back(tm);
}

void CActivityList::AddSolidTime(int tm)
{
	solidTime.push_back(tm);
}

void CActivityList::AddOpenServerTime(int tm)
{
	openServerTime.push_back(tm);
}

bool CActivityList::operator < (const CActivityList& other) const
{
	if (id != other.id) { return id < other.id; }
	return false;
}

void CActivityBreakdown::AddDayBreakdownList(int idex, int tm)
{
	MapIntVecTemplate<int>::iterator it = dayBreakdownList.find(idex);
	if (it == dayBreakdownList.end())
	{
		VectorTemplate<int> tmp;
		tmp.push_back(tm);
		dayBreakdownList.insert({ idex , tmp });
	}
	else
	{
		it->second.push_back(tm);
	}
}

void CActivityBreakdown::AddHourBreakdownList(int idex, int tm)
{
	MapIntVecTemplate<int>::iterator it = hourBreakdownList.find(idex);
	if (it == hourBreakdownList.end())
	{
		VectorTemplate<int> tmp;
		tmp.push_back(tm);
		hourBreakdownList.insert({ idex , tmp });
	}
	else
	{
		it->second.push_back(tm);
	}
}

int CActivityBreakdown::GetDayBrushMonsterCfgid(int& index) const
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

int CActivityBreakdown::GetHourBrushMonsterCfgid(int& index) const
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

int CActivityBreakdown::GetDayBrushMonsterCfg(int index) const
{
	MapIntVecTemplate<int>::const_iterator it = dayBreakdownList.find(index);
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

int CActivityBreakdown::GetHourBrushMonsterCfg(int index) const
{
	MapIntVecTemplate<int>::const_iterator it = hourBreakdownList.find(index);
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

bool CActivityBreakdown::operator < (const CActivityBreakdown& other) const
{
	if (id != other.id) { return id < other.id; }
	return false;
}

SetTemplate<CActivityList>& ActivityHallCfg::GetActivityListCfgSet()
{
	return m_ActivityListCfgSet;
}

const CActivityBreakdown* ActivityHallCfg::GetActivityBreakdown(int id)
{
	SetTemplate<CActivityBreakdown>::iterator it = m_ActivityBreakdownCfgSet.find(id);
	if (it == m_ActivityBreakdownCfgSet.end())
	{
		return nullptr;
	}
	return &(*it);
}

VectorTemplate<CBrushMonsterCfg>* ActivityHallCfg::GetBrushMonsterCfg(int id)
{
	MapIntVecTemplate<CBrushMonsterCfg>::iterator it = m_BrushMonsterCfgMap.find(id);
	if (it == m_BrushMonsterCfgMap.end())
	{
		return nullptr;
	}

	return &it->second;
}

void ActivityHallCfg::ReadActivityBreakdownCfg(CActivityBreakdown* config)
{
	m_ActivityBreakdownCfgSet.insert(*config);
}

void ActivityHallCfg::ReadActivityList(CActivityList* config)
{
	m_ActivityListCfgSet.insert(*config);
}

void ActivityHallCfg::ReadBrushMonsterCfg(CBrushMonsterCfg* config)
{
	MapIntVecTemplate<CBrushMonsterCfg>::iterator it = m_BrushMonsterCfgMap.find(config->id);
	if (it == m_BrushMonsterCfgMap.end())
	{
		VectorTemplate<CBrushMonsterCfg> tmp;
		tmp.push_back(*config);
		m_BrushMonsterCfgMap.insert({ config->id, tmp });
	}
	else
	{
		it->second.push_back(*config);
	}
}