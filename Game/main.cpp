#include "stdafx.h"

int main()
{
	bool run = true;
	ServiceType type = ServiceType::SERVICE_TYPE_GAMECENTER;

	LogMgr.SetLogFileType(type);

	if (!LuaMgr.InitCfgMgr())
	{
		COUT_LOG(LOG_CERROR, "main exit");
		return -1;
	}

	IDGen& idGen = Util::Instance()->GetIDGen();
	idGen.Init((int)type, BaseCfgMgr.GetServerId());

	TCPClient clien;

	if (!clien.InitTCPClient(type))
	{
		COUT_LOG(LOG_CERROR, "main exit");
		return -1;
	}

	LogMgr.Init(run);

	Util::Exit(run, clien);

	return 0;
}