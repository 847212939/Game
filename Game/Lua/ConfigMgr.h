#pragma once

class ConfigMgr
{
public:
	ConfigMgr();
	virtual~ConfigMgr();

protected:
	ConfigMgr(const ConfigMgr& my);
	ConfigMgr& operator=(const ConfigMgr& my);

public:
	// ��־��ӡ
	void CoutLog(int level, const char* log);

public:
	// ��������
	CBaseCfgMgr& GetCBaseCfgMgr();
	void ReadLogicCfg(CLogicCfg* config);
	void ReadDBServerCfg(CLogicCfg* config);
	void ReadThreadCntCfg(int cnt);
	void ReadServerIDCfg(int id);
	void ReadLogPrintTmCfg(int tm);
	void ReadMaxSocketCntCfg(int cnt);
	void ReadExitCfg(std::string exit);
	void ReadTimerCntCfg(int cnt);
	void ReadKeyCfg(std::string key);
	void ReadOpenServerTime(std::string servertm);

public:
	// �
	void ReadActivityList(CActivityList* config);
	void ReadBrushMonsterCfg(CBrushMonsterCfg* config);
	void ReadActivityBreakdownCfg(CActivityBreakdown* config);

public:
	// ����
	void ReadHeroList(CHeroList* config);
	void ReadSkillIdList(CSkillIdList* config);

private:
	CBaseCfgMgr	m_CBaseCfgMgr;

public:
	BuildReference(ActivityHallCfg, ActivityHallCfg)
	BuildReference(SkillCfg, SkillCfg)
};

// tolua++.exe -o LuaCfgMgr.cpp Config.pkg