#include "../Game/stdafx.h"

ActiveAlways::ActiveAlways()
{
}

ActiveAlways::~ActiveAlways()
{
}

bool ActiveAlways::Enter(ActivityList* cfg)
{
	COUT_LOG(LOG_CINFO, "ActiveAlways::Enter");
	return false;
}

bool ActiveAlways::Exit(ActivityList* cfg)
{
	COUT_LOG(LOG_CINFO, "ActiveAlways::Exit");
	return false;
}