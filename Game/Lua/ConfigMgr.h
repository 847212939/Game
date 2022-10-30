#pragma once

class ConfigMgr
{
public:
	ConfigMgr();
	virtual~ConfigMgr();

protected:
	ConfigMgr(const ConfigMgr& my);
	ConfigMgr& operator=(const ConfigMgr& my);

public:
	// 日志打印
	void CoutLog(int level, const char* log);

public:
	// 基础配置
	CBaseCfgMgr& GetCBaseCfgMgr();
	void ReadLogicCfg(CLogicCfg* config);
	void ReadDBServerCfg(CLogicCfg* config);
	void ReadThreadCntCfg(int cnt);
	void ReadServerIDCfg(int id);
	void ReadLogPrintTmCfg(int tm);
	void ReadMaxSocketCntCfg(int cnt);
	void ReadExitCfg(std::string exit);
	void ReadTimerCntCfg(int cnt);
	void ReadKeyCfg(std::string key);
	void ReadOpenServerTime(std::string servertm);

public:
	// 活动
	void ReadActivityList(CActivityList* config);
	void ReadBrushMonsterCfg(CBrushMonsterCfg* config);
	void ReadActivityBreakdownCfg(CActivityBreakdown* config);

public:
	// 技能
	void ReadHeroList(CHeroList* config);
	void ReadSkillIdList(CSkillIdList* config);

private:
	CBaseCfgMgr	m_CBaseCfgMgr;

public:
	BuildReference(ActivityHallCfg, ActivityHallCfg)
	BuildReference(SkillCfg, SkillCfg)
};

// tolua++.exe -o LuaCfgMgr.cpp Config.pkg