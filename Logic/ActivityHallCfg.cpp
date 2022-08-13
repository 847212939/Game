#include "../Game/stdafx.h"

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