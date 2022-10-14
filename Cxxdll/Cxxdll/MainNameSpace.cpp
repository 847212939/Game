#include "pch.h"

namespace MainNameSpace
{
    Util* g_Util = nullptr;

    EXPORT_DLL int EXPORT_STDCALL InitNetwork(char* ip, int port, int timerCnt)
    {
        if (!g_Util)
        {
            g_Util = Util::Instance();
        }
        std::string& encryptkey = Util::GetEncryptKey();
        if (encryptkey.empty())
        {
            Util::SetEncryptKey("LiuDaNaoDai");
        }
        TCPClient* pTcpClient = g_Util->GetTCPClient();
        if (!pTcpClient)
        {
            return -1;
        }
        if (!ip || port <= 0)
        {
            return -1;
        }
        if (!pTcpClient->InitNetwork(ip, port, timerCnt))
        {
            return -1;
        }
        
        return 0;
    }

    EXPORT_DLL int EXPORT_STDCALL InitCxxnet(NetworkCallBackFunc netFunc, TimerCallBackFunc timerFunc, CloseCallBackFunc closeFunc)
    {
        if (!g_Util->InitCxxnet(netFunc, timerFunc, closeFunc))
        {
            return -1;
        }
        return 0;
    }

    EXPORT_DLL int EXPORT_STDCALL UnInitCxxnet()
    {
        TCPClient* pTcpClient = g_Util->GetTCPClient();
        if (!pTcpClient)
        {
            return -1;
        }

        pTcpClient->CloseSocketCallback(nullptr);

        return 0;
    }

    EXPORT_DLL int EXPORT_STDCALL SendMsg(char* pData, int size, int mainID, int assistID, int uIdentification)
    {
        TCPClient* pTcpClient = g_Util->GetTCPClient();
        if (!pTcpClient)
        {
            return -1;
        }
        if (!pTcpClient->SendMsg((const char*)pData, (size_t)size, mainID, assistID, 0,
            (unsigned int)uIdentification))
        {
            return -1;
        }
        return 0;
    }

    EXPORT_DLL int EXPORT_STDCALL RegisterTimers(int timerid, int uElapse)
    {
        TCPClient* pTcpClient = g_Util->GetTCPClient();
        if (!pTcpClient)
        {
            return -1;
        }
        if (!pTcpClient->SetTimer(timerid, uElapse, SERVERTIMER_TYPE_PERISIST))
        {
            return -1;
        }
        pTcpClient->AddTimerCallback(timerid);

        return 0;
    }

    EXPORT_DLL int EXPORT_STDCALL UnRegisterTimers(int timerid)
    {
        TCPClient* pTcpClient = g_Util->GetTCPClient();
        if (!pTcpClient)
        {
            return -1;
        }
        if (!pTcpClient->KillTimer(timerid))
        {
            return -1;
        }
        pTcpClient->DelTimerCallback(timerid);

        return 0;
    }
}