#pragma once

// tolua++.exe -o Lua_ConfigMgr.cpp Config.pkg

class ConfigMgr
{
public:
	ConfigMgr();
	~ConfigMgr();

	void Run();
	void Run(int a);
};