#include "../stdafx.h"

ActiveService::ActiveService(PlayerPrepClient* ppc) : m_bmid(0)
{
}

ActiveService::~ActiveService()
{
}

bool ActiveService::Enter(CActivityList* cfg)
{
	ActivityHallSys& activeSys = G_SceneClient->GetActivityHallSys();
	if (!activeSys.Enter(cfg, m_bmid))
	{
		return false;
	}

	return true;
}

bool ActiveService::Exit(CActivityList* cfg)
{
	ActivityHallSys& activeSys = G_SceneClient->GetActivityHallSys();
	if (!activeSys.Exit(cfg, m_bmid))
	{
		return false;
	}
	
	return true;
}