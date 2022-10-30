#include "stdafx.h"

bool run = true;

int main()
{
	ServiceType type = ServiceType::SERVICE_TYPE_DB;

	G_LogMgr->SetLogFileType(type);

	if (!G_LuaMgr->InitCfgMgr())
	{
		Log(CERR, "main exit");
		return -1;
	}
	if (!G_NetClient->Init(type))
	{
		Log(CERR, "main exit");
		return -1;
	}

	G_LogMgr->Init(std::ref(run));
	Util::Exit(std::ref(run));

	return 0;
}