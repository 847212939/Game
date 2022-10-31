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
	void ReadServerCfg(CServerCfg* config);
	void ReadDbCfg(CDbCfg* config);
	void ReadThreadCntCfg(int cnt);
	void ReadLogPrintTmCfg(int tm);
	void ReadMaxSocketCntCfg(int cnt);
	void ReadExitCfg(std::string exit);

private:
	CBaseCfgMgr	m_CBaseCfgMgr;
};

// tolua++.exe -o LuaCfgMgr.cpp Config.pkg