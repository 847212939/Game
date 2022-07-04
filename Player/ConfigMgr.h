#pragma once

// tolua++.exe -o Lua_ConfigMgr.h Config.pkg

class ConfigMgr
{
public:
	static ConfigMgr* Instance();

public:
	void Run();
	void Run(int a);

private:
	ConfigMgr();
	~ConfigMgr();
};