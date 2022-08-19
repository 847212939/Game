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

void ConfigMgr::ReadDbCfg(CDbCfg* config) 
{ 
	m_CBaseCfgMgr.ReadDbCfg(config); 
}

void ConfigMgr::ReadThreadCntCfg(int cnt) 
{ 
	m_CBaseCfgMgr.ReadThreadCntCfg(cnt); 
}

void ConfigMgr::ReadServerIDCfg(int id)
{
	m_CBaseCfgMgr.ReadServerIDCfg(id);
}

void ConfigMgr::ReadLogPrintTmCfg(int tm)
{
	m_CBaseCfgMgr.ReadLogPrintTmCfg(tm);
}

void ConfigMgr::CoutLog(const char* log)
{
	std::cout << log << std::endl;
}

void ConfigMgr::ReadMaxSocketCntCfg(int cnt)
{
	m_CBaseCfgMgr.ReadMaxSocketCntCfg(cnt);
}

void ConfigMgr::ReadExitCfg(std::string exit)
{
	m_CBaseCfgMgr.ReadExitCfg(exit);
}

void ConfigMgr::ReadTimerCntCfg(int cnt)
{
	m_CBaseCfgMgr.ReadTimerCntCfg(cnt);
}

void ConfigMgr::ReadKeyCfg(std::string key)
{
	m_CBaseCfgMgr.ReadKeyCfg(key);
}

void ConfigMgr::ReadOpenServerTime(std::string servertm)
{
	m_CBaseCfgMgr.ReadOpenServerTime(servertm);
}