#pragma once

class ConfigMgr
{
public:
	ConfigMgr();
	virtual~ConfigMgr();

public:
	// »’÷æ¥Ú”°
	void CoutLog(int level, const char* log);

public:
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

private:
	CBaseCfgMgr m_CBaseCfgMgr;
	
};

// tolua++.exe -o LuaCfgMgr.cpp Config.pkg