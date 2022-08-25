#include "pch.h"

namespace MainNameSpace
{
    EXPORT_DLL int EXPORT_STDCALL InitNetwork(char* ip, int port, int timerCnt)
    {
        if (!ip || port <= 0)
        {
            return -1;
        }
        TCPClient* pTcpClient = Util::Instance()->GetTCPClient();
        if (!pTcpClient)
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
        if (!Util::Instance()->InitCxxnet(netFunc, timerFunc, closeFunc))
        {
            return -1;
        }
        return 0;
    }

    EXPORT_DLL int EXPORT_STDCALL UnInitCxxnet()
    {
        TCPClient* pTcpClient = Util::Instance()->GetTCPClient();
        if (!pTcpClient)
        {
            std::cout << "!pTcpClient" << std::endl;
            return -1;
        }

        std::cout << "SafeDelete(pTcpClient);" << std::endl;

        SafeDelete(pTcpClient);

        return 0;
    }

    EXPORT_DLL int EXPORT_STDCALL SendData(char* pData, int size, int mainID, int assistID, int uIdentification)
    {
        TCPClient* pTcpClient = Util::Instance()->GetTCPClient();
        if (!pTcpClient)
        {
            return -1;
        }
        if (!pTcpClient->SendData((const char*)pData, (size_t)size, mainID, assistID, 0,
            pTcpClient->GetTCPSocketInfo().bev, (unsigned int)uIdentification))
        {
            return -1;
        }
        return 0;
    }

    EXPORT_DLL int EXPORT_STDCALL RegisterTimers(int timerid, int uElapse)
    {
        TCPClient* pTcpClient = Util::Instance()->GetTCPClient();
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
        TCPClient* pTcpClient = Util::Instance()->GetTCPClient();
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