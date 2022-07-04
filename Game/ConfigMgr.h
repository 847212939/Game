#pragma once

// tolua++.exe -o Lua_ConfigMgr.h Config.pkg

class ConfigMgr
{
public:
	ConfigMgr();
	~ConfigMgr();

	void Run();
	void Run(int a);
};