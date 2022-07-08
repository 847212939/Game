#pragma once

// tolua++.exe -o LuaCfgMgr.cpp Config.pkg

class ConfigMgr
{
public:
	ConfigMgr();
	~ConfigMgr();

public:
	CBaseCfgMgr& GetCBaseCfgMgr();
	void ReadLogicCfg(LogicCfg* config);
	void ReadDbCfg(DbCfg* config);
	void ReadThreadCntCfg(int cnt);

private:
	CBaseCfgMgr m_CBaseCfgMgr;
};