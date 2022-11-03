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
        if (!pTcpClient->InitNetwork(ClientInfo(ip, port, timerCnt)))
        {
            return -1;
        }

        return 0;
    }

    EXPORT_DLL int EXPORT_STDCALL InitCxxnet(NetworkCallBackFunc netFunc, TimerCallBackFunc timerFunc, CloseCallBackFunc closeFunc)
    {
        if (!g_Util)
        {
            return -1;
        }
        TCPClient* pTcpClient = g_Util->GetTCPClient();
        if (!pTcpClient)
        {
            return -1;
        }
        pTcpClient->InitCallBack(netFunc, timerFunc, closeFunc);

        if (!pTcpClient->Init(ServiceType::SERVICE_TYPE_BEGIN))
        {
            return -1;
        }
        if (!pTcpClient->ConnectServer())
        {
            return -1;
        }
        if (!pTcpClient->GetIsConnect())
        {
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
        if (!pTcpClient->GetIsConnect())
        {
            return -1;
        }

        return 0;
    }

    EXPORT_DLL int EXPORT_STDCALL UnInitCxxnet()
    {
        if (!g_Util)
        {
            return -1;
        }
        TCPClient* pTcpClient = g_Util->GetTCPClient();
        if (!pTcpClient)
        {
            return -1;
        }

        pTcpClient->Stop();
        pTcpClient->NotifyAll();

        return 0;
    }

    EXPORT_DLL int EXPORT_STDCALL SendMsg(char* pData, int size, int mainID, int assistID, int uIdentification)
    {
        if (!g_Util)
        {
            return -1;
        }
        TCPClient* pTcpClient = g_Util->GetTCPClient();
        if (!pTcpClient)
        {
            return -1;
        }
        SOCKFD serverIndex = pTcpClient->GetServerIndex();
        if (serverIndex < 0)
        {
            return -1;
        }
        TCPSocketInfo* pTcpInfo = pTcpClient->GetTCPSocketInfo((int)serverIndex);
        if (!pTcpInfo)
        {
            return -1;
        }
        if (!pTcpClient->SendMsg((int)serverIndex, pData, (size_t)size,
            (MsgCmd)mainID, assistID, 0, pTcpInfo->bev, uIdentification))
        {
            return -1;
        }
        return 0;
    }

    EXPORT_DLL int EXPORT_STDCALL RegisterTimers(int timerid, int uElapse)
    {
        if (!g_Util)
        {
            return -1;
        }
        TCPClient* pTcpClient = g_Util->GetTCPClient();
        if (!pTcpClient)
        {
            return -1;
        }

        RegisterTimer(pTcpClient->GetPlayerPrepClient(), 
            (TimerCmd)timerid, uElapse, SERVERTIMER_TYPE_PERISIST);

        return 0;
    }

    EXPORT_DLL int EXPORT_STDCALL UnRegisterTimers(int timerid)
    {
        if (!g_Util)
        {
            return -1;
        }
        TCPClient* pTcpClient = g_Util->GetTCPClient();
        if (!pTcpClient)
        {
            return -1;
        }
        
        UnRegisterTimer(pTcpClient->GetPlayerPrepClient(), (TimerCmd)timerid);

        return 0;
    }
}