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
	CBaseCfgMgr() : m_serverID(0), m_threadCnt(0){}
	~CBaseCfgMgr(){}

public:
	void ReadLogicCfg(LogicCfg* config) { m_LogicCfg = *config; }
	void ReadDbCfg(DbCfg* config) { m_DbCfg = *config; }
	void ReadThreadCntCfg(int cnt) { m_threadCnt = cnt; }
	void ReadServerIDCfg(int id) { m_serverID = id; }

public:
	const LogicCfg& GetLogicCfg() { return m_LogicCfg; }
	const DbCfg& GetDbCfg() { return m_DbCfg; }
	int GetThreadCnt() { return m_threadCnt; }
	int GetServerId() { return m_serverID; }

private:
	int			m_serverID;
	int			m_threadCnt;
	LogicCfg	m_LogicCfg;
	DbCfg		m_DbCfg;
};
