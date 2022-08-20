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
	// ��������
	CBaseCfgMgr& GetCBaseCfgMgr();
	void ReadLogicCfg(CLogicCfg* config);
	void ReadTimerCntCfg(int cnt);
	void ReadKeyCfg(std::string key);

private:
	CBaseCfgMgr	m_CBaseCfgMgr;
};

// tolua++.exe -o LuaCfgMgr.cpp Config.pkg