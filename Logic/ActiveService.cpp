#include "../Game/stdafx.h"

ActiveService::ActiveService(PlayerPrepClient* ppc) : m_bmid(0)
{
}

ActiveService::~ActiveService()
{
}

bool ActiveService::Enter(ActivityList* cfg)
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

bool ActiveService::Exit(ActivityList* cfg)
{
	ActivityHallSys& activeSys = DSC->GetActivityHallSys();
	ActivityHallCfg& activityHallCfg = CfgMgr->GetActivityHallCfg();
	CfgVector<BrushMonsterCfg>* pVector = activityHallCfg.GetBrushMonsterCfg(m_bmid);
	if (!pVector)
	{
		COUT_LOG(LOG_CINFO, "pVector = null");
		return false;
	}
	for (auto& config : *pVector)
	{
		RefMonsterKey key(config.mid, config.x, config.y);
		std::vector<Animal*>* pValue = activeSys.GetRefMonster(config.sid, key);
		if (!pValue)
		{
			continue;
		}
		while (!pValue->empty())
		{
			Animal* pAnimal = pValue->back();
			if (pAnimal)
			{
				DSC->DelSceneAnimalMap(config.sid, pAnimal);
				SafeDelete(pAnimal);
			}
			pValue->pop_back();
		}
		pValue->clear();

		activeSys.DelRefMonster(config.sid, key);
	}

	return true;
}