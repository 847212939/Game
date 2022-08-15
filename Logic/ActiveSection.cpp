#include "../Game/stdafx.h"

ActiveSection::ActiveSection(PlayerPrepClient* ppc) : m_bmid(0)
{
}

ActiveSection::~ActiveSection()
{
}

// 活动区间进入函数
bool ActiveSection::Enter(ActivityList* cfg)
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

bool ActiveSection::Exit(ActivityList* cfg)
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