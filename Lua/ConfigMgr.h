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
	// ��־��ӡ
	void CoutLog(int level, const char* log);

public:
	// ��������
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
	// �
	ActivityHallCfg& GetActivityHallCfg();
	void ReadActivityList(ActivityList* config);
	void ReadBrushMonsterCfg(BrushMonsterCfg* config);
	void ReadActivityBreakdownCfg(ActivityBreakdown* config);

private:
	CBaseCfgMgr		m_CBaseCfgMgr;
	ActivityHallCfg m_ActivityHallCfgMgr;
	
};

// tolua++.exe -o LuaCfgMgr.cpp Config.pkg