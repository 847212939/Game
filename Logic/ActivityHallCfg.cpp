#include "../Game/stdafx.h"

CfgSet<ActivityList>& ActivityHallCfg::GetActivityListCfgSet()
{
	return m_ActivityListCfgSet;
}

void ActivityHallCfg::ReadActivityBreakdownCfg(ActivityBreakdown* config)
{
	CfgMap<ActivityBreakdown>::iterator it = m_ActivityBreakdownCfgMap.find(config->id);
	if (it == m_ActivityBreakdownCfgMap.end())
	{
		CfgVector<ActivityBreakdown> tmp;
		tmp.push_back(*config);
		m_ActivityBreakdownCfgMap.insert({ config->id, tmp });
	}
	else
	{
		it->second.push_back(*config);
	}
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