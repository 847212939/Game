#pragma once

struct CLogicCfg
{
	std::string ip;
	int port;

	CLogicCfg() : ip(""), port(0) {}
	~CLogicCfg() {}
};

class CBaseCfgMgr
{
public:
	CBaseCfgMgr();
	virtual~CBaseCfgMgr();

protected:
	CBaseCfgMgr(const CBaseCfgMgr& my);
	CBaseCfgMgr& operator=(const CBaseCfgMgr& my);

public:
	void ReadTimerCntCfg(int cnt);
	void ReadKeyCfg(std::string key);
	void ReadLogicCfg(CLogicCfg* config);

public:
	int GetServerId();
	int GetTimerCnt();
	int GetThreadCnt();
	int GetLogPrintTm();
	int GetMaxSocketCnt();
	std::string GetExit();
	const CLogicCfg& GetLogicCfg();
	std::string GetKey();
	std::string GetOpenServerTime();

private:
	int			m_TimerCnt;
	CLogicCfg	m_LogicCfg;
	std::string m_Key;
};
