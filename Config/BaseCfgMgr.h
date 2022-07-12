#pragma once

struct LogicCfg
{
	std::string ip;
	int port;
	int maxSocketCnt;

	LogicCfg() : ip(""), port(0), maxSocketCnt(0) {}
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
	CBaseCfgMgr() : m_serverID(0), m_threadCnt(0), m_logPrintTm(0) {}
	~CBaseCfgMgr(){}

public:
	void ReadServerIDCfg(int id)		{ m_serverID = id; }
	void ReadThreadCntCfg(int cnt)		{ m_threadCnt = cnt; }
	void ReadLogPrintTmCfg(int tm)		{ m_logPrintTm = tm; }
	void ReadDbCfg(DbCfg* config)		{ m_DbCfg = *config; }
	void ReadLogicCfg(LogicCfg* config) { m_LogicCfg = *config; }

public:
	int GetServerId()					{ return m_serverID; }
	int GetThreadCnt()					{ return m_threadCnt; }
	int GetLogPrintTm()					{ return m_logPrintTm; }
	const DbCfg& GetDbCfg()				{ return m_DbCfg; }
	const LogicCfg& GetLogicCfg()		{ return m_LogicCfg; }

private:
	int			m_logPrintTm;
	int			m_serverID;
	int			m_threadCnt;
	LogicCfg	m_LogicCfg;
	DbCfg		m_DbCfg;
};
