#include "../Game/stdafx.h"

ActiveAlways::ActiveAlways(PlayerPrepClient* ppc)
{
}

ActiveAlways::~ActiveAlways()
{
}

bool ActiveAlways::Enter(ActivityList* cfg)
{
	ActivityHallSys& activeSys = DSC->GetActivityHallSys();
	CfgVector<BrushMonsterCfg>* pVector = activeSys.Enter(cfg);
	if (!pVector)
	{
		COUT_LOG(LOG_CINFO, "pVector = null");
		return false;
	}
	for (auto& cfg : *pVector)
	{
		RefMonsterKey key(cfg.mid, cfg.x, cfg.y);
		std::vector<Animal*>* pValue = activeSys.GetRefMonsterVec(cfg.sid, key);
		if (!(pValue ? activeSys.CreateMonster(pValue, cfg) : activeSys.InitMonster(cfg)))
		{
			continue;
		}
	}

	return true;
}

bool ActiveAlways::Exit(ActivityList* cfg)
{
	COUT_LOG(LOG_CINFO, "ActiveAlways::Exit");
	return false;
}