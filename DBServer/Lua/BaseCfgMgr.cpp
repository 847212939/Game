#include "../stdafx.h"

CBaseCfgMgr::CBaseCfgMgr() : 
	m_threadCnt(0), 
	m_logPrintTm(0), 
	m_maxSocketCnt(0), 
	m_Exit("")
{

}

CBaseCfgMgr::~CBaseCfgMgr() 
{

}

void CBaseCfgMgr::ReadThreadCntCfg(int cnt) 
{
	m_threadCnt = cnt; 
}
void CBaseCfgMgr::ReadLogPrintTmCfg(int tm) 
{
	m_logPrintTm = tm; 
}
void CBaseCfgMgr::ReadMaxSocketCntCfg(int cnt) 
{
	m_maxSocketCnt = cnt;
}
void CBaseCfgMgr::ReadExitCfg(std::string exit) 
{
	m_Exit = exit; 
}
void CBaseCfgMgr::ReadDbCfg(CDbCfg* config) 
{ 
	m_DbCfg = *config; 
}
void CBaseCfgMgr::ReadServerCfg(CServerCfg* config)
{ 
	m_CServerCfg = *config;
}

int CBaseCfgMgr::GetThreadCnt() 
{
	return m_threadCnt;
}
int CBaseCfgMgr::GetLogPrintTm() 
{ 
	return m_logPrintTm; 
}
int CBaseCfgMgr::GetMaxSocketCnt() 
{ 
	return m_maxSocketCnt;
}
std::string CBaseCfgMgr::GetExit() 
{ 
	return m_Exit; 
}
const CDbCfg& CBaseCfgMgr::GetDbCfg() 
{ 
	return m_DbCfg;
}
const CServerCfg& CBaseCfgMgr::GetServerCfg()
{ 
	return m_CServerCfg; 
}
