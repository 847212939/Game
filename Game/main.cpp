#include "stdafx.h"

int main()
{
	bool run = true;
	ServiceType type = ServiceType::SERVICE_TYPE_GAMECENTER;

	LogMgr->SetLogFileType(type);

	if (!LuaMgr->InitCfgMgr())
	{
		COUT_LOG(LOG_CERROR, "main exit");
		return -1;
	}

	IDGen& idGen = DUtil.GetIDGen();
	idGen.Init((int)type, BaseCfgMgr.GetServerId());

	if (!DTCPC->Init(type))
	{
		COUT_LOG(LOG_CERROR, "main exit");
		return -1;
	}

	LogMgr->Init(run);

	Util::Exit(run);

	return 0;
}