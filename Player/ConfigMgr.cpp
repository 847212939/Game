#include "../Game/stdafx.h"

ConfigMgr* ConfigMgr::Instance()
{
	static ConfigMgr g_mgr;
	return &g_mgr;
}

ConfigMgr::ConfigMgr()
{
}

void ConfigMgr::Run()
{
	COUT_LOG(LOG_CINFO, "ConfigMgr Run");
}

void ConfigMgr::Run(int a)
{
	COUT_LOG(LOG_CINFO, "ConfigMgr Run");
}

ConfigMgr::~ConfigMgr()
{
}