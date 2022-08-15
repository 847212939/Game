#include "../Game/stdafx.h"

ActiveTime::ActiveTime(PlayerPrepClient* ppc) : m_bmid(0)
{
}

ActiveTime::~ActiveTime()
{
}

bool ActiveTime::Enter(ActivityList* cfg)
{
	ActivityHallSys& activeSys = DSC->GetActivityHallSys();
	CfgVector<BrushMonsterCfg>* pVector = activeSys.GetBrushMonsterCfg(cfg, m_bmid);
	if (!pVector)
	{
		COUT_LOG(LOG_CINFO, "pVector = null");
		return false;
	}
	for (auto& config : *pVector)
	{
		RefMonsterKey key(config.mid, config.x, config.y);
		std::vector<Animal*>* pValue = activeSys.GetRefMonster(config.sid, key);
		if (!(pValue ? activeSys.CreateMonster(pValue, config) : activeSys.InitMonster(config)))
		{
			continue;
		}
	}

	return true;
}

bool ActiveTime::Exit(ActivityList* cfg)
{
	COUT_LOG(LOG_CINFO, "ActiveTime::Exit");
	return false;
}