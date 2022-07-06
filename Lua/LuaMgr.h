#pragma once

class CLuaMgr
{
public:
	static CLuaMgr* Instance();
	bool InitCfgMgr();

	ConfigMgr* GetConfigMgr()
	{
		return m_ConfigMgr;
	}

private:
	CLuaMgr();
	virtual ~CLuaMgr();

	lua_State* m_pLua;		// vm
	ConfigMgr* m_ConfigMgr;	// ≈‰÷√π‹¿Ì¿‡
};
