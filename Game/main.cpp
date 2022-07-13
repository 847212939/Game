#include "stdafx.h"

int main()
{
	LogMgr()->SetLogFileType(ServiceType::SERVICE_TYPE_LOGIC);

	if (!LuaMgr()->InitCfgMgr())
	{
		COUT_LOG(LOG_CERROR, "main exit");
		return -1;
	}

	LogMgr()->Init();

	CBaseCfgMgr& baseCfgMgr = CfgMgr()->GetCBaseCfgMgr();

	IDGen& idGen = Util::Instance()->GetIDGen();
	idGen.Init((int)ServiceType::SERVICE_TYPE_LOGIC, baseCfgMgr.GetServerId());

	TCPClient clien;
	if (!clien.InitTCPClient())
	{
		COUT_LOG(LOG_CERROR, "main exit");
		return -1;
	}

	return 0;
}