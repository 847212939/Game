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
	void ReadLogicCfg(CLogicCfg* config);
	void ReadDbCfg(CDbCfg* config);
	void ReadThreadCntCfg(int cnt);
	void ReadServerIDCfg(int id);
	void ReadLogPrintTmCfg(int tm);
	void ReadMaxSocketCntCfg(int cnt);
	void ReadExitCfg(std::string exit);
	void ReadTimerCntCfg(int cnt);
	void ReadKeyCfg(std::string key);
	void ReadOpenServerTime(std::string servertm);

private:
	CBaseCfgMgr	m_CBaseCfgMgr;
};

// tolua++.exe -o LuaCfgMgr.cpp Config.pkg