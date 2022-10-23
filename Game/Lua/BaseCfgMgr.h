#pragma once

struct CLogicCfg
{
	std::string ip;
	int port;

	CLogicCfg() : ip(""), port(0) {}
	~CLogicCfg() {}
};

struct CDbCfg
{
	std::string ip;
	std::string user;
	std::string passwd;
	std::string database;
	int port;

	CDbCfg() : ip(""), user(""), passwd(""), database(""), port(0) {}
	~CDbCfg() {}
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
	void ReadServerIDCfg(int id);
	void ReadTimerCntCfg(int cnt);
	void ReadThreadCntCfg(int cnt);
	void ReadLogPrintTmCfg(int tm);
	void ReadMaxSocketCntCfg(int cnt);
	void ReadKeyCfg(std::string key);
	void ReadExitCfg(std::string exit);
	void ReadOpenServerTime(std::string servertm);

	void ReadDbCfg(CDbCfg* config);
	void ReadLogicCfg(CLogicCfg* config);
	void ReadDBServerCfg(CLogicCfg* config);

public:
	int GetServerId();
	int GetTimerCnt();
	int GetThreadCnt();
	int GetLogPrintTm();
	int GetMaxSocketCnt();
	std::string GetExit();
	const CDbCfg& GetDbCfg();
	const CLogicCfg& GetLogicCfg();
	const CLogicCfg& GetDBServerCfg();
	std::string GetKey();
	std::string GetOpenServerTime();

private:
	int			m_logPrintTm;
	int			m_serverID;
	int			m_threadCnt;
	int			m_maxSocketCnt;
	int			m_TimerCnt;

	CLogicCfg	m_LogicCfg;
	CLogicCfg	m_DBServerCfg;
	CDbCfg		m_DbCfg;
	std::string m_Exit;
	std::string m_Key;
	std::string m_Servertm;
};
