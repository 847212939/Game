#pragma once

struct LogicCfg
{
	std::string ip;
	int port;

	LogicCfg() : ip(""), port(0) {}
	~LogicCfg() {}
};

struct DbCfg
{
	std::string ip;
	std::string user;
	std::string passwd;
	std::string database;
	int port;

	DbCfg() : ip(""), user(""), passwd(""), database(""), port(0) {}
	~DbCfg() {}
};

class CBaseCfgMgr
{
public:
	CBaseCfgMgr();
	virtual~CBaseCfgMgr();

public:
	void ReadServerIDCfg(int id);
	void ReadTimerCntCfg(int cnt);
	void ReadThreadCntCfg(int cnt);
	void ReadLogPrintTmCfg(int tm);
	void ReadMaxSocketCntCfg(int cnt);
	void ReadKeyCfg(std::string key);
	void ReadExitCfg(std::string exit);

	void ReadDbCfg(DbCfg* config);
	void ReadLogicCfg(LogicCfg* config);

public:
	int GetServerId();
	int GetTimerCnt();
	int GetThreadCnt();
	int GetLogPrintTm();
	int GetMaxSocketCnt();
	std::string GetExit();
	const DbCfg& GetDbCfg();
	const LogicCfg& GetLogicCfg();
	std::string GetKey();

private:
	int			m_logPrintTm;
	int			m_serverID;
	int			m_threadCnt;
	int			m_maxSocketCnt;
	int			m_TimerCnt;

	LogicCfg	m_LogicCfg;
	DbCfg		m_DbCfg;
	std::string m_Exit;
	std::string m_Key;
};
