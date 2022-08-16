#include "../Game/stdafx.h"

ActiveAlways::ActiveAlways(PlayerPrepClient* ppc) : m_bmid(0)
{
}

ActiveAlways::~ActiveAlways()
{
}

// 全天活动回调
bool ActiveAlways::Enter(CActivityList* cfg)
{
	ActivityHallSys& activeSys = DSC->GetActivityHallSys();
	if (!activeSys.Enter(cfg, m_bmid))
	{
		return false;
	}

	return true;
}

bool ActiveAlways::Exit(CActivityList* cfg)
{
	COUT_LOG(LOG_CINFO, "ActiveAlways::Exit");
	return false;
}