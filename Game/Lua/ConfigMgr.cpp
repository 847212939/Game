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

// 基础配置
void ConfigMgr::ReadLogicCfg(CLogicCfg* config) 
{ 
	m_CBaseCfgMgr.ReadLogicCfg(config);
}
void ConfigMgr::ReadDBServerCfg(CLogicCfg* config)
{
	m_CBaseCfgMgr.ReadDBServerCfg(config);
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

// 活动
void ConfigMgr::ReadActivityBreakdownCfg(CActivityBreakdown* config)
{
	m_ActivityHallCfg.ReadActivityBreakdownCfg(config);
}
void ConfigMgr::ReadActivityList(CActivityList* config)
{
	m_ActivityHallCfg.ReadActivityList(config);
}
void ConfigMgr::ReadBrushMonsterCfg(CBrushMonsterCfg* config)
{
	m_ActivityHallCfg.ReadBrushMonsterCfg(config);
}

// 技能
void ConfigMgr::ReadHeroList(CHeroList* config)
{
	m_SkillCfg.ReadHeroList(config);
}
void ConfigMgr::ReadSkillIdList(CSkillIdList* config)
{
	m_SkillCfg.ReadSkillIdList(config);
}