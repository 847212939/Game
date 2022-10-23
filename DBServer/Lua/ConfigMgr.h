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