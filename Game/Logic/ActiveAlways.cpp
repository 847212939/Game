#include "../stdafx.h"

ActiveAlways::ActiveAlways(PlayerPrepClient* ppc) : m_bmid(0)
{
}

ActiveAlways::~ActiveAlways()
{
}

// 全天活动回调
bool ActiveAlways::Enter(CActivityList* cfg)
{
	ActivityHallSys& activeSys = G_SceneClient->GetActivityHallSys();
	if (!activeSys.Enter(cfg, m_bmid))
	{
		return false;
	}

	return true;
}

bool ActiveAlways::Exit(CActivityList* cfg)
{
	Log(CINF, "ActiveAlways::Exit");
	return false;
}