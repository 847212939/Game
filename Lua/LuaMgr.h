#pragma once

class CLuaMgr
{
public:
	static CLuaMgr* Instance();

public:
	bool InitCfgMgr();
    ConfigMgr* GetConfigMgr();

private:
	CLuaMgr();
	virtual ~CLuaMgr();

private:
	lua_State* m_pLua;		// vm
	ConfigMgr* m_ConfigMgr;	// ≈‰÷√π‹¿Ì¿‡
};
