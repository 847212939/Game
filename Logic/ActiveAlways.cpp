#include "../Game/stdafx.h"

ActiveAlways::ActiveAlways(PlayerPrepClient* ppc) : m_bmid(0)
{
}

ActiveAlways::~ActiveAlways()
{
}

// 全天活动回调
bool ActiveAlways::Enter(ActivityList* cfg)
{
	ActivityHallSys& activeSys = DSC->GetActivityHallSys();
	CfgVector<BrushMonsterCfg>* pVector = activeSys.GetBrushMonsterCfgVec(cfg, m_bmid);
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

bool ActiveAlways::Exit(ActivityList* cfg)
{
	COUT_LOG(LOG_CINFO, "ActiveAlways::Exit");
	return false;
}