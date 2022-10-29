// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"

//#include "mainspace.h"
//using namespace MainNameSpace;
//bool run = false;
//
//void NetworkCallBack1(REvent eve)
//{
//    std::cout << "NetworkCallBack1" << std::endl;
//}
//
//void TimerCallBack1(int timer)
//{
//    std::cout << "TimerCallBack1" << std::endl;
//}
//
//void CloseCallBackFunc1()
//{
//    std::cout << "CloseCallBackFunc1" << std::endl;
//    run = false;
//}
//
//int InitSocket(std::string ip, int port, int timerCnt)
//{
//    if (run)
//    {
//        return -1;
//    }
//    if (InitNetwork((char*)ip.c_str(), port, timerCnt) != 0)
//    {
//        return -1;
//    }
//    if (InitCxxnet(NetworkCallBack1, TimerCallBack1, CloseCallBackFunc1) != 0)
//    {
//        return -1;
//    }
//
//    run = true;
//
//    return 0;
//}
//
//int main()
//{
//    
//    if (InitSocket((char*)"127.0.0.1", 8888, 1) != 0)
//    {
//        std::cout << "初始化网络失败" << std::endl;
//        return -1;
//    }
//
//    RegisterTimers(1, 1000);
//    
//    SendMsg((char*)"", 0, 7, 1, 3);
//
//
//    while (true)
//    {
//        if (!run)
//        {
//            if (InitSocket((char*)"127.0.0.1", 8888, 1) != 0)
//            {
//                std::cout << "初始化网络失败" << std::endl;
//            }
//        }
//
//        std::this_thread::sleep_for(std::chrono::seconds(1));
//    }
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

