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
	CBaseCfgMgr(){}
	~CBaseCfgMgr(){}

	void ReadLogicCfg(LogicCfg* config)
	{
		m_LogicCfg = *config;
	}

	void ReadDbCfg(DbCfg* config)
	{
		m_DbCfg = *config;
	}

	const LogicCfg& GetLogicCfg()
	{
		return m_LogicCfg;
	}

	const DbCfg& GetDbCfg()
	{
		return m_DbCfg;
	}

private:
	LogicCfg	m_LogicCfg;
	DbCfg		m_DbCfg;
};
