#include "pch.h"

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

void ConfigMgr::ReadLogicCfg(CLogicCfg* config) 
{ 
	m_CBaseCfgMgr.ReadLogicCfg(config);
}

void ConfigMgr::ReadTimerCntCfg(int cnt)
{
	m_CBaseCfgMgr.ReadTimerCntCfg(cnt);
}

void ConfigMgr::ReadKeyCfg(std::string key)
{
	m_CBaseCfgMgr.ReadKeyCfg(key);
}
