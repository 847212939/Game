#include "../stdafx.h"

ActiveTime::ActiveTime(PlayerPrepClient* ppc) : m_bmid(0)
{
}

ActiveTime::~ActiveTime()
{
}

bool ActiveTime::Enter(CActivityList* cfg)
{
	ActivityHallSys& activeSys = DSC->GetActivityHallSys();
	if (!activeSys.Enter(cfg, m_bmid))
	{
		return false;
	}

	return true;
}

bool ActiveTime::Exit(CActivityList* cfg)
{
	COUT_LOG(LOG_CINFO, "ActiveTime::Exit");
	return false;
}