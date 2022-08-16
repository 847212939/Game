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

void ConfigMgr::ReadOpenServerTime(std::string servertm)
{
	m_CBaseCfgMgr.ReadOpenServerTime(servertm);
}

void ConfigMgr::ReadActivityBreakdownCfg(ActivityBreakdown* config)
{
	m_ActivityHallCfg.ReadActivityBreakdownCfg(config);
}

void ConfigMgr::ReadActivityList(ActivityList* config)
{
	m_ActivityHallCfg.ReadActivityList(config);
}

void ConfigMgr::ReadBrushMonsterCfg(BrushMonsterCfg* config)
{
	m_ActivityHallCfg.ReadBrushMonsterCfg(config);
}

void ConfigMgr::ReadHeroList(HeroList* config)
{
	m_SkillCfg.ReadHeroList(config);
}

void ConfigMgr::ReadSkillIdList(SkillIdList* config)
{
	m_SkillCfg.ReadSkillIdList(config);
}