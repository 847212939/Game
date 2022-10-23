#pragma once

struct CServerCfg
{
	std::string ip;
	int port;

	CServerCfg() : ip(""), port(0) {}
	~CServerCfg() {}
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
	void ReadThreadCntCfg(int cnt);
	void ReadLogPrintTmCfg(int tm);
	void ReadMaxSocketCntCfg(int cnt);
	void ReadExitCfg(std::string exit);

	void ReadDbCfg(CDbCfg* config);
	void ReadServerCfg(CServerCfg* config);

public:
	int GetThreadCnt();
	int GetLogPrintTm();
	int GetMaxSocketCnt();
	std::string GetExit();
	const CDbCfg& GetDbCfg();
	const CServerCfg& GetServerCfg();

private:
	int			m_logPrintTm;
	int			m_threadCnt;
	int			m_maxSocketCnt;

	CServerCfg	m_LogicCfg;
	CDbCfg		m_DbCfg;
	std::string m_Exit;
};
