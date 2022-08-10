#include "../Game/stdafx.h"

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

void ConfigMgr::ReadLogicCfg(LogicCfg* config) 
{ 
	m_CBaseCfgMgr.ReadLogicCfg(config);
}

void ConfigMgr::ReadDbCfg(DbCfg* config) 
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

void ConfigMgr::ReadTimerCntCfg(int cnt)
{
	m_CBaseCfgMgr.ReadTimerCntCfg(cnt);
}

void ConfigMgr::ReadKeyCfg(std::string key)
{
	m_CBaseCfgMgr.ReadKeyCfg(key);
}

ActivityHallCfg& ConfigMgr::GetActivityHallCfg()
{
	return m_ActivityHallCfgMgr;
}

void ConfigMgr::ReadMiningCfg(MiningCfg* config)
{
	m_ActivityHallCfgMgr.ReadMiningCfg(config);
}

void ConfigMgr::ReadActivityList(ActivityList* config)
{
	m_ActivityHallCfgMgr.ReadActivityList(config);
}

void ConfigMgr::ReadBrushMonsterCfg(BrushMonsterCfg* config)
{
	m_ActivityHallCfgMgr.ReadBrushMonsterCfg(config);
}