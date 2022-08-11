#include "../Game/stdafx.h"

ActiveTime::ActiveTime()
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