#include "../stdafx.h"

ConfigMgr::ConfigMgr()
{
}
ConfigMgr::~ConfigMgr()
{
}

CBaseCfgMgr& ConfigMgr::GetCBaseCfgMgr() 
{
	return m_CBaseCfgMgr; 
}

// ª˘¥°≈‰÷√
void ConfigMgr::ReadServerCfg(CServerCfg* CServerCfg)
{ 
	m_CBaseCfgMgr.ReadServerCfg(config);
}
void ConfigMgr::ReadDbCfg(CDbCfg* config) 
{ 
	m_CBaseCfgMgr.ReadDbCfg(config); 
}
void ConfigMgr::ReadThreadCntCfg(int cnt) 
{ 
	m_CBaseCfgMgr.ReadThreadCntCfg(cnt); 
}
void ConfigMgr::ReadLogPrintTmCfg(int tm)
{
	m_CBaseCfgMgr.ReadLogPrintTmCfg(tm);
}
void ConfigMgr::CoutLog(int level, const char* log)
{
	COUT_LOG(level, log);
}
void ConfigMgr::ReadMaxSocketCntCfg(int cnt)
{
	m_CBaseCfgMgr.ReadMaxSocketCntCfg(cnt);
}
void ConfigMgr::ReadExitCfg(std::string exit)
{
	m_CBaseCfgMgr.ReadExitCfg(exit);
}
