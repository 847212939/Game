// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"

//bool run = true;
//int main()
//{
//	ServiceType type = ServiceType::SERVICE_TYPE_DB;
//
//	G_LogMgr->SetLogFileType(type);
//
//	if (!G_LuaMgr->InitCfgMgr())
//	{
//		Log(CERR, "main exit");
//		return -1;
//	}
//	if (!G_NetClient->Init(type))
//	{
//		Log(CERR, "main exit");
//		return -1;
//	}
//
//	G_LogMgr->Init(std::ref(run));
//	Util::Exit(std::ref(run));
//
//	return 0;
//}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

