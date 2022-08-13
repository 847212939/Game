#include "../Game/stdafx.h"

ActiveTime::ActiveTime(PlayerPrepClient* ppc)
{
}

ActiveTime::~ActiveTime()
{
}

bool ActiveTime::Enter(ActivityList* cfg)
{
	COUT_LOG(LOG_CINFO, "ActiveTime::Enter");
	return false;
}

bool ActiveTime::Exit(ActivityList* cfg)
{
	COUT_LOG(LOG_CINFO, "ActiveTime::Exit");
	return false;
}