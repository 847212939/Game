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
	void ReadLogicCfg(LogicCfg* config);
	void ReadDbCfg(DbCfg* config);
	void ReadThreadCntCfg(int cnt);
	void ReadServerIDCfg(int id);
	void ReadLogPrintTmCfg(int tm);
	void ReadMaxSocketCntCfg(int cnt);
	void ReadExitCfg(std::string exit);
	void ReadTimerCntCfg(int cnt);
	void ReadKeyCfg(std::string key);

public:
	// 活动
	ActivityHallCfg& GetActivityHallCfg();
	void ReadActivityList(ActivityList* config);
	void ReadBrushMonsterCfg(BrushMonsterCfg* config);
	void ReadActivityBreakdownCfg(ActivityBreakdown* config);

private:
	CBaseCfgMgr		m_CBaseCfgMgr;
	ActivityHallCfg m_ActivityHallCfgMgr;
	
};

// tolua++.exe -o LuaCfgMgr.cpp Config.pkg