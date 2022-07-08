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
	CBaseCfgMgr() : m_threadCnt(0){}
	~CBaseCfgMgr(){}

public:
	void ReadLogicCfg(LogicCfg* config) { m_LogicCfg = *config; }
	void ReadDbCfg(DbCfg* config) { m_DbCfg = *config; }
	void ReadThreadCntCfg(int cnt) { m_threadCnt = cnt; }

public:
	const LogicCfg& GetLogicCfg() { return m_LogicCfg; }
	const DbCfg& GetDbCfg() { return m_DbCfg; }
	int GetThreadCnt() { return m_threadCnt; }

private:
	int			m_threadCnt;
	LogicCfg	m_LogicCfg;
	DbCfg		m_DbCfg;
};
