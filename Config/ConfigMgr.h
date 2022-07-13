#pragma once

// tolua++.exe -o LuaCfgMgr.cpp Config.pkg

class ConfigMgr
{
public:
	ConfigMgr();
	~ConfigMgr();

public:
	void CoutLog(const char* log);

public:
	CBaseCfgMgr& GetCBaseCfgMgr();
	void ReadLogicCfg(LogicCfg* config);
	void ReadDbCfg(DbCfg* config);
	void ReadThreadCntCfg(int cnt);
	void ReadServerIDCfg(int id);
	void ReadLogPrintTmCfg(int tm);

private:
	CBaseCfgMgr m_CBaseCfgMgr;
};