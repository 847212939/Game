#pragma once

// tolua++.exe -o LuaCfgMgr.cpp Config.pkg

class ConfigMgr
{
public:
	ConfigMgr();
	~ConfigMgr();

	CBaseCfgMgr& GetCBaseCfgMgr() { return m_CBaseCfgMgr; }
	void ReadLogicCfg(LogicCfg* config) { m_CBaseCfgMgr.ReadLogicCfg(config); }
	void ReadDbCfg(DbCfg* config) { m_CBaseCfgMgr.ReadDbCfg(config); }
private:
	CBaseCfgMgr m_CBaseCfgMgr;
};