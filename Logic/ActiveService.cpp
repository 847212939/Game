#include "../Game/stdafx.h"

ActiveService::ActiveService()
{
}

ActiveService::~ActiveService()
{
}

bool ActiveService::Enter(ActivityList* cfg)
{
	COUT_LOG(LOG_CINFO, "ActiveService::Enter");
	return false;
}

bool ActiveService::Exit(ActivityList* cfg)
{
	COUT_LOG(LOG_CINFO, "ActiveService::Exit");
	return false;
}