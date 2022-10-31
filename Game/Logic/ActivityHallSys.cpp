#include "../stdafx.h"

ActivityHallSys::ActivityHallSys(PlayerPrepClient* ppc) :
	m_ActiveSection(ppc),
	m_ActiveAlways(ppc),
	m_ActiveService(ppc),
	m_ActiveTime(ppc)
{
	RegisterActive(ActivityHallSys::AtSectionOpen, ActType::at_section_open);
	RegisterActiveEnter(ActiveSection::Enter, ActType::at_section_open, m_ActiveSection);
	RegisterActiveExit(ActiveSection::Exit, ActType::at_section_open, m_ActiveSection);

	RegisterActive(ActivityHallSys::AtAlwaysOpen, ActType::at_always_open);
	RegisterActiveEnter(ActiveAlways::Enter, ActType::at_always_open, m_ActiveAlways);
	RegisterActiveExit(ActiveAlways::Exit, ActType::at_always_open, m_ActiveAlways);

	RegisterActive(ActivityHallSys::AtServiceOpen, ActType::at_service_open);
	RegisterActiveEnter(ActiveService::Enter, ActType::at_service_open, m_ActiveService);
	RegisterActiveExit(ActiveService::Exit, ActType::at_service_open, m_ActiveService);

	RegisterActive(ActivityHallSys::AtTimedOpen, ActType::at_timed_open);
	RegisterActiveEnter(ActiveTime::Enter, ActType::at_timed_open, m_ActiveTime);
	RegisterActiveExit(ActiveTime::Exit, ActType::at_timed_open, m_ActiveTime);

	//RegisterTimer(ppc, ActivityHallSys::TimerCallback, TimerCmd::TimerCmd_Active, 300, SERVERTIMER_TYPE_PERISIST);
}

ActivityHallSys::~ActivityHallSys()
{

}

// 活动是否开启
bool ActivityHallSys::GetActiveOpen(int id)
{
	MapActiveOpen::iterator it = m_ActtiveOpenMap.find(id);
	if (it == m_ActtiveOpenMap.end())
	{
		return false;
	}
	return it->second.open;
}

VectorTemplate<CBrushMonsterCfg>* ActivityHallSys::GetBrushMonsterCfg(CActivityList* cfg, int& bmid)
{
	std::pair<int, int> pr; // type, index
	ActivityHallCfg& activityHallCfg = G_CfgMgr->GetActivityHallCfg();

	const CActivityBreakdown* pConfig = activityHallCfg.GetActivityBreakdown(cfg->activityBreakdown);
	if (!pConfig)
	{
		Log(CINF, "pConfig = null");
		return nullptr;
	}

	bmid = GetBrushMonsterId(pConfig, pr);
	if (bmid <= 0)
	{
		Log(CINF, "bmid <= 0");
		return nullptr;
	}

	// 清除上一次场景中怪物
	ClearBrushMonsterCfgVec(pConfig, pr);

	return activityHallCfg.GetBrushMonsterCfg(bmid);
}

// 清除上一次场景中怪物
void ActivityHallSys::ClearBrushMonsterCfgVec(const CActivityBreakdown* pConfig, std::pair<int, int>& pr)
{
	ActivityHallCfg& activityHallCfg = G_CfgMgr->GetActivityHallCfg(); 
	for (int i = 1; i < pr.second; i++)
	{
		VectorTemplate<CBrushMonsterCfg>* pVector = 
			activityHallCfg.GetBrushMonsterCfg(GetPreBrushMonsterId(pConfig, pr.first, i));
		if (!pVector)
		{
			continue;
		}
		for (auto& config : *pVector)
		{
			RefMonsterKey key(config.mid, config.x, config.y);
			std::vector<Animal*>* pValue = GetRefMonster(config.sid, key);
			if (!pValue)
			{
				continue;
			}
			while (!pValue->empty())
			{
				Animal* pAnimal = pValue->back();
				if (pAnimal)
				{
					G_SceneClient->DelSceneAnimalMap(config.sid, pAnimal);
					SafeDelete(pAnimal);
				}
				pValue->pop_back();
			}
			pValue->clear();

			DelRefMonster(config.sid, key);
		}
	}
}

bool ActivityHallSys::InitMonster(CBrushMonsterCfg& cfg)
{
	time_t cur = ::time(nullptr);
	std::vector<Animal*> animalVec;

	for (int i = 0; i < cfg.count; i++)
	{
		Animal* animal = Util::CreatAnimal(AnimalType::at_monster, cfg.mid);

		cfg.delayTime > 0 ? 
		animal->SetLived(false),
		animal->SetResuTime(cur + cfg.delayTime): 
		animal->SetLived(true);
		animal->SetRefreshTime(cfg.refreshTime);

		if (!G_SceneClient->EnterScene(animal, cfg.sid, Transform(cfg.x, cfg.y)))
		{
			SafeDelete(animal);
			Log(CERR, "进入场景失败,请检查代码或者配置,重启服务器");
			return false;
		}

		animalVec.push_back(animal);
	}

	RefMonsterKey key(cfg.mid, cfg.x, cfg.y);
	AddRefMonster(cfg.sid, key, animalVec);
	
	return true;
}

bool ActivityHallSys::CreateMonster(std::vector<Animal*>* pValue, CBrushMonsterCfg& cfg)
{
	if (!pValue || pValue->empty())
	{
		Log(CERR, "pValue = null");
		return false;
	}
	time_t cur = ::time(nullptr);
	for (auto& animal : *pValue)
	{
		if (animal->GetLived())
		{
			continue;
		}
		if (cur < animal->GetResuTime())
		{
			continue;
		}
		animal->SetLived(true);
	}

	return true;
}

std::vector<Animal*>* ActivityHallSys::GetRefMonster(int sid, RefMonsterKey& key)
{
	MapMonster::iterator it = m_MonsterMap.find(sid);
	if (it == m_MonsterMap.end())
	{
		return nullptr;
	}
	else
	{
		MapMonsterKV::iterator pos = it->second.find(key);
		if (pos == it->second.end())
		{
			return nullptr;
		}
		return &pos->second;
	}

	return nullptr;
}

void ActivityHallSys::AddRefMonster(int sid, RefMonsterKey& key, std::vector<Animal*>& value)
{
	MapMonster::iterator it = m_MonsterMap.find(sid);
	if (it == m_MonsterMap.end())
	{
		MapMonsterKV tmpMonsterKVMap;
		tmpMonsterKVMap.insert({ key, value });
		m_MonsterMap.insert({ sid, tmpMonsterKVMap });
	}
	else
	{
		MapMonsterKV::iterator pos = it->second.find(key);
		if (pos == it->second.end())
		{
			it->second.insert({ key, value });
		}
		else
		{
			// 出错才会走到这里面来
			while (!pos->second.empty())
			{
				Animal* pAnimal = pos->second.back();
				if (pAnimal)
				{
					G_SceneClient->DelSceneAnimalMap(sid, pAnimal);
					SafeDelete(pAnimal);
				}
				pos->second.pop_back();
			}
			pos->second.clear();
			pos->second.swap(value);
			Log(CERR, "add monster is err sid = %d, mid = %d", sid, key.mid);
		}
	}
}

void ActivityHallSys::DelRefMonster(int sid, RefMonsterKey& key)
{
	MapMonster::iterator it = m_MonsterMap.find(sid);
	if (it == m_MonsterMap.end())
	{
		return;
	}
	else
	{
		MapMonsterKV::iterator pos = it->second.find(key);
		if (pos == it->second.end())
		{
			return;
		}
		it->second.erase(pos);
	}
}

// 活动类型判断函数回调
bool ActivityHallSys::ActiveCallBackFun(ActType type, CActivityList* cfg)
{
	MapActivityFunc::iterator it = m_ActivityFunMap.find(type);
	if (it == m_ActivityFunMap.end())
	{
		Log(CERR, "No corresponding callback function found cmd = %d", type);
		return false;
	}

	return it->second(cfg);
}

// 活动类型判断
void ActivityHallSys::AddActiveCallback(ActType type, std::function<bool(CActivityList*)>&& fun)
{
	MapActivityFunc::iterator it = m_ActivityFunMap.find(type);
	if (it == m_ActivityFunMap.end())
	{
		m_ActivityFunMap.insert(std::make_pair(type, fun));
		return;
	}

	Log(CERR, "There is already a callback for this message. Please check the code cmd = %d", type);
}

// 进入活动场景回调
bool ActivityHallSys::ActiveEnterCallBackFun(ActType type, CActivityList* cfg)
{
	MapActivityFunc::iterator it = m_ActivityEnterFunMap.find(type);
	if (it == m_ActivityEnterFunMap.end())
	{
		Log(CERR, "No corresponding callback function found cmd = %d", type);
		return false;
	}

	return it->second(cfg);
}

void ActivityHallSys::AddActiveEnterCallback(ActType type, std::function<bool(CActivityList*)>&& fun)
{
	MapActivityFunc::iterator it = m_ActivityEnterFunMap.find(type);
	if (it == m_ActivityEnterFunMap.end())
	{
		m_ActivityEnterFunMap.insert(std::make_pair(type, fun));
		return;
	}

	Log(CERR, "There is already a callback for this message. Please check the code cmd = %d", type);
}

bool ActivityHallSys::ActiveExitCallBackFun(ActType type, CActivityList* cfg)
{
	MapActivityFunc::iterator it = m_ActivityExitFunMap.find(type);
	if (it == m_ActivityExitFunMap.end())
	{
		Log(CERR, "No corresponding callback function found cmd = %d", type);
		return false;
	}

	return it->second(cfg);
}

void ActivityHallSys::AddActiveExitCallback(ActType type, std::function<bool(CActivityList*)>&& fun)
{
	MapActivityFunc::iterator it = m_ActivityExitFunMap.find(type);
	if (it == m_ActivityExitFunMap.end())
	{
		m_ActivityExitFunMap.insert(std::make_pair(type, fun));
		return;
	}

	Log(CERR, "There is already a callback for this message. Please check the code cmd = %d", type);
}

// 时间区间
bool ActivityHallSys::AtSectionOpen(CActivityList* cfg)
{
	if (!cfg)
	{
		return false;
	}
	uint64_t sysSecond = Util::GetSysSecond();
	uint64_t endSecond = Util::GetCfgSecond(cfg->endTime);
	uint64_t beginSecond = Util::GetCfgSecond(cfg->beginTime);
	
	if (beginSecond > endSecond)
	{
		if (sysSecond >= endSecond && sysSecond < beginSecond)
		{
			return false;
		}
		return true;
	}
	else
	{
		if (sysSecond >= beginSecond && sysSecond < endSecond)
		{
			return true;
		}
		return false;
	}

	return false;
}

int ActivityHallSys::GetBrushMonsterId(const CActivityBreakdown* pConfig, std::pair<int, int>& pr)
{
	pr.first = pConfig->dayBreakdown - pConfig->hourBreakdown > 0 ? 1 : 2;
	if (pr.first == 1)
	{
		return pConfig->GetDayBrushMonsterCfgid(pr.second);
	}
	else if (pr.first == 2)
	{
		return pConfig->GetHourBrushMonsterCfgid(pr.second);
	}
	return 0;
}

int ActivityHallSys::GetPreBrushMonsterId(const CActivityBreakdown* pConfig, int type, int index)
{
	if (type == 1)
	{
		return pConfig->GetDayBrushMonsterCfg(index);
	}
	else if (type == 2)
	{
		return pConfig->GetHourBrushMonsterCfg(index);
	}
	
	return 0;
}

// 定时器回调
void ActivityHallSys::TimerCallback()
{
	for (auto& cfg : G_CfgMgr->GetActivityHallCfg().GetActivityListCfgSet())
	{
		if (!ActiveCallBackFun((ActType)cfg.type, const_cast<CActivityList*>(&cfg)))
		{
			// 活动结束
			MapActiveOpen::iterator it = m_ActtiveOpenMap.find(cfg.id);
			if (it == m_ActtiveOpenMap.end())
			{
				continue;
			}
			if (!it->second.open)
			{
				continue;
			}
			ActiveExitCallBackFun((ActType)cfg.type, const_cast<CActivityList*>(&cfg));
			it->second.open = false;
			continue;
		}

		// 活动开启
		MapActiveOpen::iterator it = m_ActtiveOpenMap.find(cfg.id);
		if (it == m_ActtiveOpenMap.end())
		{
			m_ActtiveOpenMap.insert({ cfg.id, ActtiveOpen(cfg.id, true) });
		}
		else
		{
			it->second.open = true;
		}
		ActiveEnterCallBackFun((ActType)cfg.type, const_cast<CActivityList*>(&cfg));
	}
}

bool ActivityHallSys::Enter(CActivityList* cfg, int& bmid)
{
	VectorTemplate<CBrushMonsterCfg>* pVector = GetBrushMonsterCfg(cfg, bmid);
	if (!pVector)
	{
		Log(CINF, "pVector = null");
		return false;
	}
	for (auto& config : *pVector)
	{
		RefMonsterKey key(config.mid, config.x, config.y);
		std::vector<Animal*>* pValue = GetRefMonster(config.sid, key);
		if (!(pValue ? CreateMonster(pValue, config) : InitMonster(config)))
		{
			continue;
		}
	}

	return true;
}

bool ActivityHallSys::Exit(CActivityList* cfg, const int& bmid)
{
	ActivityHallCfg& activityHallCfg = G_CfgMgr->GetActivityHallCfg();
	VectorTemplate<CBrushMonsterCfg>* pVector = activityHallCfg.GetBrushMonsterCfg(bmid);
	if (!pVector)
	{
		Log(CINF, "pVector = null");
		return false;
	}
	for (auto& config : *pVector)
	{
		RefMonsterKey key(config.mid, config.x, config.y);
		std::vector<Animal*>* pValue = GetRefMonster(config.sid, key);
		if (!pValue)
		{
			continue;
		}
		while (!pValue->empty())
		{
			Animal* pAnimal = pValue->back();
			if (pAnimal)
			{
				G_SceneClient->DelSceneAnimalMap(config.sid, pAnimal);
				SafeDelete(pAnimal);
			}
			pValue->pop_back();
		}
		pValue->clear();

		DelRefMonster(config.sid, key);
	}

	return true;
}


// 全天开启
bool ActivityHallSys::AtAlwaysOpen(CActivityList* cfg)
{
	if (!cfg)
	{
		return false;
	}

	uint64_t endSecond = Util::GetCfgSecond(cfg->endTime);
	uint64_t beginSecond = Util::GetCfgSecond(cfg->beginTime);

	if (endSecond == beginSecond)
	{
		return true;
	}

	return false;
}

// 开服活动
bool ActivityHallSys::AtServiceOpen(CActivityList* cfg)
{
	if (!cfg)
	{
		return false;
	}
	if (cfg->openServerTime.size() < 4)
	{
		return false;
	}

	uint64_t openServerTime = Util::GetCfgSecond(cfg->openServerTime);
	uint64_t curTime = ::time(nullptr) - Util::GetOpenServerTime();
	
	if (curTime <= 0)
	{
		return false;
	}
	if (curTime >= openServerTime && curTime < openServerTime + cfg->openServerTime[3] * 60)
	{
		return true;
	}

	return false;
}

// 固定时间开启
bool ActivityHallSys::AtTimedOpen(CActivityList* cfg)
{
	if (!cfg)
	{
		return false;
	}

	uint64_t sysSecond = Util::GetSysSecond();
	uint64_t solidSecond = Util::GetCfgSecond(cfg->solidTime);

	if (sysSecond == solidSecond)
	{
		return true;
	}

	return false;
}
