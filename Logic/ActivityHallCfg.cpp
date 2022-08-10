#include "../Game/stdafx.h"

void ActivityHallCfg::ReadMiningCfg(MiningCfg* config)
{
	m_MiningCfgSet.insert(*config);
}

void ActivityHallCfg::ReadActivityList(ActivityList* config)
{
	m_ActivityListCfgSet.insert(*config);
}

void ActivityHallCfg::ReadBrushMonsterCfg(BrushMonsterCfg* config)
{
	BrushMonsterCfgMap::iterator it = m_BrushMonsterCfgMap.find(config->id);
	if (it == m_BrushMonsterCfgMap.end())
	{
		CfgVector<BrushMonsterCfg> tmp;
		tmp.push_back(*config);
		m_BrushMonsterCfgMap.insert({config->id, tmp});
	}
	else
	{
		it->second.push_back(*config);
	}
}