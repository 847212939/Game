#include "../Game/stdafx.h"

ActiveSection::ActiveSection(PlayerPrepClient* ppc)
{
}

ActiveSection::~ActiveSection()
{
}

// 活动区间进入函数
bool ActiveSection::Enter(ActivityList* cfg)
{
	ActivityHallSys& activeSys = DSC->GetActivityHallSys();
	CfgVector<BrushMonsterCfg>* pVector = activeSys.GetBrushMonsterCfgVec(cfg);
	if (!pVector)
	{
		COUT_LOG(LOG_CINFO, "pVector = null");
		return false;
	}
	for (auto& config : *pVector)
	{
		RefMonsterKey key(config.mid, config.x, config.y);
		std::vector<Animal*>* pValue = activeSys.GetRefMonsterVec(config.sid, key);
		if (!(pValue ? activeSys.CreateMonster(pValue, config) : activeSys.InitMonster(config)))
		{
			continue;
		}
	}

	return true;
}

bool ActiveSection::Exit(ActivityList* cfg)
{
	COUT_LOG(LOG_CINFO, "ActiveSection::Exit");
	return false;
}