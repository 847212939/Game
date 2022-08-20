#include "pch.h"

CBaseCfgMgr::CBaseCfgMgr() : 
	m_TimerCnt(0) 
{

}

CBaseCfgMgr::~CBaseCfgMgr() 
{

}

void CBaseCfgMgr::ReadTimerCntCfg(int cnt) 
{
	m_TimerCnt = cnt;
}

void CBaseCfgMgr::ReadKeyCfg(std::string key)
{
	m_Key = key;
}

void CBaseCfgMgr::ReadLogicCfg(CLogicCfg* config) 
{ 
	m_LogicCfg = *config; 
}

int CBaseCfgMgr::GetTimerCnt() 
{ 
	return m_TimerCnt; 
}

const CLogicCfg& CBaseCfgMgr::GetLogicCfg() 
{ 
	return m_LogicCfg; 
}

std::string CBaseCfgMgr::GetKey()
{
	return m_Key;
}