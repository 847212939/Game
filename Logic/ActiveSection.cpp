#include "../Game/stdafx.h"

ActiveSection::ActiveSection()
{
}

ActiveSection::~ActiveSection()
{
}

bool ActiveSection::Enter(ActivityList* cfg)
{
	COUT_LOG(LOG_CINFO, "ActiveSection::Enter");
	return false;
}

bool ActiveSection::Exit(ActivityList* cfg)
{
	COUT_LOG(LOG_CINFO, "ActiveSection::Exit");
	return false;
}