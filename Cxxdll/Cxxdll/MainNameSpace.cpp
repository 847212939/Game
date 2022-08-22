#include "pch.h"

namespace MainNameSpace
{
    bool InitNetwork(char* ip, int port, int timerCnt)
    {
        if (!ip)
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

    void SendData(char* pData, int size, int mainID, int assistID, int uIdentification)
    {
        TCPClient* pTcpClient = Util::Instance()->GetTCPClient();
        if (!pTcpClient)
        {
            return;
        }
        pTcpClient->SendData((const char*)pData, (size_t)size, mainID, assistID, 0, 
            pTcpClient->GetTCPSocketInfo().bev, (unsigned int)uIdentification);
    }

    void RegisterTimers(int timerid, int uElapse)
    {
        TCPClient* pTcpClient = Util::Instance()->GetTCPClient();
        if (!pTcpClient)
        {
            return;
        }
        pTcpClient->SetTimer(timerid, uElapse, SERVERTIMER_TYPE_PERISIST);
        pTcpClient->AddTimerCallback(timerid);
    }

    void UnRegisterTimers(int timerid)
    {
        TCPClient* pTcpClient = Util::Instance()->GetTCPClient();
        if (!pTcpClient)
        {
            return;
        }
        pTcpClient->KillTimer(timerid);
        pTcpClient->DelTimerCallback(timerid);
    }
}