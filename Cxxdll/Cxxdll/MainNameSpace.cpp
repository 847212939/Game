#include "pch.h"

namespace MainNameSpace
{
    bool InitNetwork(char* ip, int port, int timerCnt)
    {
        if (!ip || port <= 0)
        {
            return false;
        }
        TCPClient* pTcpClient = Util::Instance()->GetTCPClient();
        if (!pTcpClient)
        {
            return false;
        }
        return pTcpClient->InitNetwork(ip, port, timerCnt);
    }

    bool InitCxxnet(NetworkCallBackFunc netFunc, TimerCallBackFunc timerFunc)
    {
        return Util::Instance()->InitCxxnet(netFunc, timerFunc);
    }

    bool SendData(char* pData, int size, int mainID, int assistID, int uIdentification)
    {
        TCPClient* pTcpClient = Util::Instance()->GetTCPClient();
        if (!pTcpClient)
        {
            return false;
        }
        return pTcpClient->SendData((const char*)pData, (size_t)size, mainID, assistID, 0,
            pTcpClient->GetTCPSocketInfo().bev, (unsigned int)uIdentification);
    }

    bool RegisterTimers(int timerid, int uElapse)
    {
        TCPClient* pTcpClient = Util::Instance()->GetTCPClient();
        if (!pTcpClient)
        {
            return false;
        }
        if (!pTcpClient->SetTimer(timerid, uElapse, SERVERTIMER_TYPE_PERISIST))
        {
            return false;
        }
        pTcpClient->AddTimerCallback(timerid);

        return true;
    }

    bool UnRegisterTimers(int timerid)
    {
        TCPClient* pTcpClient = Util::Instance()->GetTCPClient();
        if (!pTcpClient)
        {
            return false;
        }
        if (!pTcpClient->KillTimer(timerid))
        {
            return false;
        }
        pTcpClient->DelTimerCallback(timerid);

        return true;
    }
}