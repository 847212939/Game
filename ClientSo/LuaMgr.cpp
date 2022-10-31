#include "pch.h"

CLuaMgr::CLuaMgr() : m_ConfigMgr(new ConfigMgr)
{

}

CLuaMgr::~CLuaMgr()
{
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
    return true;
}
