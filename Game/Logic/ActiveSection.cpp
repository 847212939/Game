#include "../stdafx.h"

ActiveSection::ActiveSection(PlayerPrepClient* ppc) : m_bmid(0)
{
}

ActiveSection::~ActiveSection()
{
}

// 活动区间进入函数
bool ActiveSection::Enter(CActivityList* cfg)
{
	ActivityHallSys& activeSys = G_SceneClient->GetActivityHallSys();
	if (!activeSys.Enter(cfg, m_bmid))
	{
		return false;
	}

	return true;
}

bool ActiveSection::Exit(CActivityList* cfg)
{
	ActivityHallSys& activeSys = G_SceneClient->GetActivityHallSys();
	if (!activeSys.Exit(cfg, m_bmid))
	{
		return false;
	}

	return true;
}