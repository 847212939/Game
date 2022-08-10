#include "../Game/stdafx.h"

ActivityHallSys::ActivityHallSys(PlayerPrepClient* ppc)
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

bool ActivityHallSys::GetActiveOpen(int id)
{
	ActtiveOpenMap::iterator it = m_ActtiveOpenMap.find(id);
	if (it == m_ActtiveOpenMap.end())
	{
		return false;
	}
	return it->second.open;
}

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

void ActivityHallSys::TimerCallback()
{
	for (auto& cfg : CfgMgr->GetActivityHallCfg().GetActivityListCfgSet())
	{
		if (!ActiveCallBackFun((ActType)cfg.type, const_cast<ActivityList*>(&cfg)))
		{
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

		ActtiveOpenMap::iterator it = m_ActtiveOpenMap.find(cfg.id);
		if (it == m_ActtiveOpenMap.end())
		{
			m_ActtiveOpenMap.insert({ cfg.id, ActtiveOpen(cfg.id, true) });
		}
		if (!it->second.open)
		{
			it->second.open = true;
		}
		ActiveEnterCallBackFun((ActType)cfg.type, const_cast<ActivityList*>(&cfg));
	}

	// 定时器反注册
	UnRegisterTimer(DPPC, TimerCmd::TimerCmd_Active);
}
