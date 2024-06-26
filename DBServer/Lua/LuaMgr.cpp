#include "../stdafx.h"

CLuaMgr::CLuaMgr() : m_pLua(nullptr), m_ConfigMgr(nullptr)
{

}

CLuaMgr::~CLuaMgr()
{
    if (m_pLua)
    {
        lua_close(m_pLua);
    }
    if (m_ConfigMgr)
    {
        SafeDelete(m_ConfigMgr);
    }
}

CLuaMgr* CLuaMgr::Instance()
{
    static CLuaMgr g_mgr;
    return &g_mgr;
}

ConfigMgr* CLuaMgr::GetConfigMgr()
{
    return m_ConfigMgr;
}

bool CLuaMgr::InitCfgMgr()
{
    if (m_pLua)
    {
        Log(CERR, "Duplicate creation m_pLua");
        return false;
    }
    if (m_ConfigMgr)
    {
        Log(CERR, "Duplicate creation m_ConfigMgr");
        return false;
    }
    int tolua_Config_open(lua_State * tolua_S);
    m_pLua = luaL_newstate();
    if (!m_pLua)
    {
        Log(CERR, "Create lua vm err");
        return false;
    }

    luaL_openlibs(m_pLua);
    tolua_Config_open(m_pLua);
    luaL_dofile(m_pLua, "Lua/LoadScript.lua");

    lua_getglobal(m_pLua, "LoadConfig");
    int iRet = lua_pcall(m_pLua, 0, 1, 0);
    if (iRet)
    {
        const char* pErrorMsg = lua_tostring(m_pLua, -1);
        Log(CERR, pErrorMsg);
        return false;
    }
    if (!lua_isuserdata(m_pLua, -1))
    {
        Log(CERR, "Lua is not user data");
        return false;
    }
    m_ConfigMgr = *(ConfigMgr**)lua_touserdata(m_pLua, -1);
    if (!m_ConfigMgr)
    {
        Log(CERR, "ConfigMgr create err");
        return false;
    }

    return true;
}
