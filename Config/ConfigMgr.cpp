#include "../Game/stdafx.h"

ConfigMgr::ConfigMgr()
{
}

ConfigMgr::~ConfigMgr()
{
}

CBaseCfgMgr& ConfigMgr::GetCBaseCfgMgr() 
{
	return m_CBaseCfgMgr; 
}

void ConfigMgr::ReadLogicCfg(LogicCfg* config) 
{ 
	m_CBaseCfgMgr.ReadLogicCfg(config);
}

void ConfigMgr::ReadDbCfg(DbCfg* config) 
{ 
	m_CBaseCfgMgr.ReadDbCfg(config); 
}

void ConfigMgr::ReadThreadCntCfg(int cnt) 
{ 
	m_CBaseCfgMgr.ReadThreadCntCfg(cnt); 
}

void ConfigMgr::ReadServerIDCfg(int id)
{
	m_CBaseCfgMgr.ReadServerIDCfg(id);
}