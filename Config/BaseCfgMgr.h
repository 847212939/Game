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
	CBaseCfgMgr() : m_serverID(0), m_threadCnt(0), m_logPrintTm(0), m_maxSocketCnt(0), m_Exit("") {}
	virtual~CBaseCfgMgr(){}

public:
	void ReadServerIDCfg(int id)		{ m_serverID = id; }
	void ReadThreadCntCfg(int cnt)		{ m_threadCnt = cnt; }
	void ReadLogPrintTmCfg(int tm)		{ m_logPrintTm = tm; }
	void ReadMaxSocketCntCfg(int cnt)	{ m_maxSocketCnt = cnt; }
	void ReadExitCfg(std::string exit)	{ m_Exit = exit; }

	void ReadDbCfg(DbCfg* config)		{ m_DbCfg = *config; }
	void ReadLogicCfg(LogicCfg* config) { m_LogicCfg = *config; }

public:
	int GetServerId()					{ return m_serverID; }
	int GetThreadCnt()					{ return m_threadCnt; }
	int GetLogPrintTm()					{ return m_logPrintTm; }
	int GetMaxSocketCnt()				{ return m_maxSocketCnt; }
	std::string GetExit()				{ return m_Exit; }
	const DbCfg& GetDbCfg()				{ return m_DbCfg; }
	const LogicCfg& GetLogicCfg()		{ return m_LogicCfg; }

private:
	int			m_logPrintTm;
	int			m_serverID;
	int			m_threadCnt;
	int			m_maxSocketCnt;

	LogicCfg	m_LogicCfg;
	DbCfg		m_DbCfg;
	std::string m_Exit;
};
