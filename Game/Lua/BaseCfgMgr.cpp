#include "../stdafx.h"

CBaseCfgMgr::CBaseCfgMgr() : 
	m_serverID(0), 
	m_threadCnt(0), 
	m_logPrintTm(0), 
	m_maxSocketCnt(0), 
	m_Exit(""), 
	m_TimerCnt(0),
	m_CrossServerID(0)
{

}

CBaseCfgMgr::~CBaseCfgMgr() 
{

}

void CBaseCfgMgr::ReadServerIDCfg(int id)
{ 
	m_serverID = id; 
}
void CBaseCfgMgr::ReadCrossServerIDCfg(int id)
{
	m_CrossServerID = id;
}
void CBaseCfgMgr::ReadTimerCntCfg(int cnt) 
{
	m_TimerCnt = cnt;
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

void CBaseCfgMgr::ReadKeyCfg(std::string key)
{
	m_Key = key;
}
void CBaseCfgMgr::ReadLogicCfg(CLogicCfg* config) 
{ 
	m_LogicCfg = *config; 
}

void CBaseCfgMgr::ReadDBServerCfg(CLogicCfg* config)
{
	m_DBServerCfg = *config;
}
void CBaseCfgMgr::ReadCrossServerCfg(CLogicCfg* config)
{
	m_CrossServerCfg = *config;
}

int CBaseCfgMgr::GetServerId() 
{ 
	return G_NetClient->GetServerType() ==
		ServiceType::SERVICE_TYPE_CROSS ?
		m_CrossServerID :
		m_serverID;
}

int CBaseCfgMgr::GetTimerCnt() 
{ 
	return m_TimerCnt; 
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
const CLogicCfg& CBaseCfgMgr::GetLogicCfg() 
{ 
	return m_LogicCfg; 
}
const CLogicCfg& CBaseCfgMgr::GetDBServerCfg()
{
	return m_DBServerCfg;
}
const CLogicCfg& CBaseCfgMgr::GetCrossServerCfg()
{
	return m_CrossServerCfg;
}
std::string CBaseCfgMgr::GetKey()
{
	return m_Key;
}

void CBaseCfgMgr::ReadOpenServerTime(std::string servertm)
{
	m_Servertm = servertm;
}

std::string CBaseCfgMgr::GetOpenServerTime()
{
	return m_Servertm;
}