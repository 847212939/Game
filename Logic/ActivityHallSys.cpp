#include "../Game/stdafx.h"

ActivityHallSys::ActivityHallSys(PlayerPrepClient* ppc)
{
	RegisterTimer(ppc, this, ActivityHallSys::TimerCallback, TimerCmd::TimerCmd_Active, 100, SERVERTIMER_TYPE_PERISIST);
	RegisterActive(this, ActivityHallSys::AtSectionOpen, ActType::at_section_open);
	RegisterActive(this, ActivityHallSys::AtAlwaysOpen, ActType::at_always_open);
	RegisterActive(this, ActivityHallSys::AtServiceOpen, ActType::at_service_open);
	RegisterActive(this, ActivityHallSys::AtTimedOpen, ActType::at_timed_open);
}

ActivityHallSys::~ActivityHallSys()
{

}

void ActivityHallSys::TimerCallback()
{
	for (auto& cfg : CfgMgr->GetActivityHallCfg().GetActivityListCfgSet())
	{
		if (!ActiveCallBackFun((ActType)cfg.type, const_cast<ActivityList*>(&cfg)))
		{
			// ��ر�
			COUT_LOG(LOG_CINFO, "��ر�");

			continue;
		}

		// �����
		COUT_LOG(LOG_CINFO, "�����");
	}

	// ��ʱ����ע��
	UnRegisterTimer(DPPC, TimerCmd::TimerCmd_Active);
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

// ʱ������
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

// ȫ�쿪��
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

// �����
bool ActivityHallSys::AtServiceOpen(ActivityList* cfg)
{
	return false;
}

// �̶�ʱ�俪��
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