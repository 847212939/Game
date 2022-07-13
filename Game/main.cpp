﻿#include "stdafx.h"

int main()
{
	bool run = true;

	LogMgr()->SetLogFileType(ServiceType::SERVICE_TYPE_LOGIC);

	if (!LuaMgr()->InitCfgMgr())
	{
		COUT_LOG(LOG_CERROR, "main exit");
		return -1;
	}

	CBaseCfgMgr& baseCfgMgr = CfgMgr()->GetCBaseCfgMgr();

	IDGen& idGen = Util::Instance()->GetIDGen();
	idGen.Init((int)ServiceType::SERVICE_TYPE_LOGIC, baseCfgMgr.GetServerId());

	TCPClient clien;
	if (!clien.InitTCPClient())
	{
		COUT_LOG(LOG_CERROR, "main exit");
		return -1;
	}

	LogMgr()->Init(run);

	while (run)
	{
		std::string str;
		std::cin >> str;

		if (str == "exit")
		{
			break;
		}
	}

	run = false;
	clien.SetRuninged(false);
	clien.NotifyAll();

	return 0;
}