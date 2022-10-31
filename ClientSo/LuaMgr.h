#pragma once

class CLuaMgr
{
public:
	static CLuaMgr* Instance();

protected:
	CLuaMgr(const CLuaMgr& my);
	CLuaMgr& operator=(const CLuaMgr& my);

public:
	bool InitCfgMgr();
    ConfigMgr* GetConfigMgr();

private:
	CLuaMgr();
	virtual ~CLuaMgr();

private:
	ConfigMgr* m_ConfigMgr;	// ≈‰÷√π‹¿Ì¿‡
};
