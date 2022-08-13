#include "../Game/stdafx.h"

ActivityHallSys::ActivityHallSys(PlayerPrepClient* ppc) :
	m_ActiveSection(ppc),
	m_ActiveAlways(ppc),
	m_ActiveService(ppc),
	m_ActiveTime(ppc)
{
	RegisterActive(this, ActivityHallSys::AtSectionOpen, ActType::at_section_open);
	RegisterActiveEnter(this, m_ActiveSection, ActiveSection::Enter, ActType::at_section_open);
	RegisterActiveExit(this, m_ActiveSection, ActiveSection::Exit, ActType::at_section_open);

	RegisterActive(this, ActivityHallSys::AtAlwaysOpen, ActType::at_always_open);
	RegisterActiveEnter(this, m_ActiveAlways, ActiveAlways::Enter, ActType::at_always_open);
	RegisterActiveExit(this, m_ActiveAlways, ActiveAlways::Exit, ActType::at_always_open);

	RegisterActive(this, ActivityHallSys::AtServiceOpen, ActType::at_service_open);
	RegisterActiveEnter(this, m_ActiveService, ActiveService::Enter, ActType::at_service_open);
	RegisterActiveExit(this, m_ActiveService, ActiveService::Exit, ActType::at_service_open);

	RegisterActive(this, ActivityHallSys::AtTimedOpen, ActType::at_timed_open);
	RegisterActiveEnter(this, m_ActiveTime, ActiveTime::Enter, ActType::at_timed_open);
	RegisterActiveExit(this, m_ActiveTime, ActiveTime::Exit, ActType::at_timed_open);

	RegisterTimer(ppc, this, ActivityHallSys::TimerCallback, TimerCmd::TimerCmd_Active, 100, SERVERTIMER_TYPE_PERISIST);
}

ActivityHallSys::~ActivityHallSys()
{

}

// 活动是否开启
bool ActivityHallSys::GetActiveOpen(int id)
{
	ActtiveOpenMap::iterator it = m_ActtiveOpenMap.find(id);
	if (it == m_ActtiveOpenMap.end())
	{
		return false;
	}
	return it->second.open;
}

int ActivityHallSys::GetBrushMonsterId(const ActivityBreakdown* pConfig)
{
	return pConfig->dayBreakdown - pConfig->hourBreakdown > 0 ?
		pConfig->GetDayBrushMonsterCfgid(Util::GetServiceDays()) :
		pConfig->GetHourBrushMonsterCfgid();
}

CfgVector<BrushMonsterCfg>* ActivityHallSys::Enter(ActivityList* cfg)
{
	const ActivityBreakdown* pConfig = CfgMgr->GetActivityHallCfg().GetActivityBreakdown(cfg->activityBreakdown);
	if (!pConfig)
	{
		COUT_LOG(LOG_CINFO, "pConfig = null");
		return nullptr;
	}

	int bmid = GetBrushMonsterId(pConfig);
	if (bmid <= 0)
	{
		COUT_LOG(LOG_CINFO, "bmid <= 0");
		return nullptr;
	}

	return CfgMgr->GetActivityHallCfg().GetBrushMonsterCfg(bmid);
}

bool ActivityHallSys::InitMonster(BrushMonsterCfg& cfg)
{
	if (cfg.delayTime > 0)
	{
		RefMonsterK key(cfg.mid, cfg.x, cfg.y);
		RefMonsterV value(cfg.count, ::time(nullptr) + cfg.delayTime);
		AddRefMonsterV(cfg.sid, key, value);
		return true;
	}
	for (int i = 0; i < cfg.count; i++)
	{
		Animal* animal = Util::CreatAnimal(AnimalType::at_monster);
		if (!DSC->EnterScene(animal, cfg.sid, Transform(cfg.x, cfg.y)))
		{
			SafeDelete(animal);
			COUT_LOG(LOG_CERROR, "进入场景失败");
			return false;
		}
		dynamic_cast<MonsterClient*>(animal)->SetMonsterid(cfg.mid);
		animal->SetLived(true);
	}

	RefMonsterK key(cfg.mid, cfg.x, cfg.y);
	RefMonsterV value(cfg.count, ::time(nullptr) + cfg.refreshTime);
	AddRefMonsterV(cfg.sid, key, value);
	
	return true;
}

bool ActivityHallSys::CreateMonster(RefMonsterV* pValue, BrushMonsterCfg& cfg)
{
	

	return true;
}

RefMonsterV* ActivityHallSys::GetRefMonsterV(int sid, RefMonsterK& key)
{
	MonsterMap::iterator it = m_MonsterMap.find(sid);
	if (it == m_MonsterMap.end())
	{
		return nullptr;
	}
	else
	{
		MonsterKVMap::iterator pos = it->second.find(key);
		if (pos == it->second.end())
		{
			return nullptr;
		}
		return &pos->second;
	}

	return nullptr;
}

void ActivityHallSys::AddRefMonsterV(int sid, RefMonsterK& key, RefMonsterV& value)
{
	MonsterMap::iterator it = m_MonsterMap.find(sid);
	if (it == m_MonsterMap.end())
	{
		MonsterKVMap tmpMonsterKVMap;
		tmpMonsterKVMap.insert({ key, value });
		m_MonsterMap.insert({ sid, tmpMonsterKVMap });
	}
	else
	{
		MonsterKVMap::iterator pos = it->second.find(key);
		if (pos == it->second.end())
		{
			it->second.insert({ key, value });
		}
		else
		{
			pos->second += value;
		}
	}
}

// 活动类型判断函数回调
bool ActivityHallSys::ActiveCallBackFun(ActType type, ActivityList* cfg)
{
	ActivityFunMap::iterator it = m_ActivityFunMap.find(type);
	if (it == m_ActivityFunMap.end())
	{
		COUT_LOG(LOG_CERROR, "No corresponding callback function found cmd = %d", type);
		return false;
	}

	return it->second(cfg);
}

// 活动类型判断
void ActivityHallSys::AddActiveCallback(ActType type, std::function<bool(ActivityList*)>&& fun)
{
	ActivityFunMap::iterator it = m_ActivityFunMap.find(type);
	if (it == m_ActivityFunMap.end())
	{
		m_ActivityFunMap.insert(std::make_pair(type, fun));
		return;
	}

	COUT_LOG(LOG_CERROR, "There is already a callback for this message. Please check the code cmd = %d", type);
}

// 进入活动场景回调
bool ActivityHallSys::ActiveEnterCallBackFun(ActType type, ActivityList* cfg)
{
	ActivityFunMap::iterator it = m_ActivityEnterFunMap.find(type);
	if (it == m_ActivityEnterFunMap.end())
	{
		COUT_LOG(LOG_CERROR, "No corresponding callback function found cmd = %d", type);
		return false;
	}

	return it->second(cfg);
}

void ActivityHallSys::AddActiveEnterCallback(ActType type, std::function<bool(ActivityList*)>&& fun)
{
	ActivityFunMap::iterator it = m_ActivityEnterFunMap.find(type);
	if (it == m_ActivityEnterFunMap.end())
	{
		m_ActivityEnterFunMap.insert(std::make_pair(type, fun));
		return;
	}

	COUT_LOG(LOG_CERROR, "There is already a callback for this message. Please check the code cmd = %d", type);
}

bool ActivityHallSys::ActiveExitCallBackFun(ActType type, ActivityList* cfg)
{
	ActivityFunMap::iterator it = m_ActivityExitFunMap.find(type);
	if (it == m_ActivityExitFunMap.end())
	{
		COUT_LOG(LOG_CERROR, "No corresponding callback function found cmd = %d", type);
		return false;
	}

	return it->second(cfg);
}

void ActivityHallSys::AddActiveExitCallback(ActType type, std::function<bool(ActivityList*)>&& fun)
{
	ActivityFunMap::iterator it = m_ActivityExitFunMap.find(type);
	if (it == m_ActivityExitFunMap.end())
	{
		m_ActivityExitFunMap.insert(std::make_pair(type, fun));
		return;
	}

	COUT_LOG(LOG_CERROR, "There is already a callback for this message. Please check the code cmd = %d", type);
}

// 时间区间
bool ActivityHallSys::AtSectionOpen(ActivityList* cfg)
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

// 全天开启
bool ActivityHallSys::AtAlwaysOpen(ActivityList* cfg)
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
bool ActivityHallSys::AtServiceOpen(ActivityList* cfg)
{
	return false;
}

// 固定时间开启
bool ActivityHallSys::AtTimedOpen(ActivityList* cfg)
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

// 定时器回调
void ActivityHallSys::TimerCallback()
{
	for (auto& cfg : CfgMgr->GetActivityHallCfg().GetActivityListCfgSet())
	{
		if (!ActiveCallBackFun((ActType)cfg.type, const_cast<ActivityList*>(&cfg)))
		{
			// 活动结束
			ActtiveOpenMap::iterator it = m_ActtiveOpenMap.find(cfg.id);
			if (it == m_ActtiveOpenMap.end())
			{
				continue;
			}
			if (!it->second.open)
			{
				continue;
			}
			ActiveExitCallBackFun((ActType)cfg.type, const_cast<ActivityList*>(&cfg));
			it->second.open = false;
			continue;
		}

		// 活动开启
		ActtiveOpenMap::iterator it = m_ActtiveOpenMap.find(cfg.id);
		if (it == m_ActtiveOpenMap.end())
		{
			m_ActtiveOpenMap.insert({ cfg.id, ActtiveOpen(cfg.id, true) });
		}
		else
		{
			it->second.open = true;
		}
		ActiveEnterCallBackFun((ActType)cfg.type, const_cast<ActivityList*>(&cfg));
	}
}
