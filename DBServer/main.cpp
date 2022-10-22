﻿#include "stdafx.h"

int main()
{
	bool run = true;
	ServiceType type = ServiceType::SERVICE_TYPE_DB;

	LogMgr->SetLogFileType(type);

	if (!LuaMgr->InitCfgMgr())
	{
		COUT_LOG(LOG_CERROR, "main exit");
		return -1;
	}
	if (!DTCPC->Init(type))
	{
		COUT_LOG(LOG_CERROR, "main exit");
		return -1;
	}

	LogMgr->Init(std::ref(run));
	Util::Exit(std::ref(run));

	return 0;
}