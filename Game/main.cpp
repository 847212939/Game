#include "stdafx.h"

// GetProcessAffinityMask
// SetProcessAffinityMask
// SetThreadAffinityMask
int main()
{
	LogMgr()->SetLogFileType(ServiceType::SERVICE_TYPE_LOGIC);
	if (!LuaMgr()->InitCfgMgr())
	{
		return -1;
	}

	CBaseCfgMgr& baseCfgMgr = CfgMgr()->GetCBaseCfgMgr();

	IDGen& idGen = Util::Instance()->GetIDGen();
	idGen.Init((int)ServiceType::SERVICE_TYPE_LOGIC, baseCfgMgr.GetServerId());

	TCPClient clien;

	return 0;
}