#include "pch.h"

namespace MainNameSpace
{
    void InitCxxnet(NetworkCallBackFunc func)
    {
        Util::Instance()->InitCxxnet(func);
    }

    void SendData(char* pData, int size, int mainID, int assistID, int uIdentification)
    {
        TCPClient* pTcpClient = Util::Instance()->GetTCPClient();
        pTcpClient->SendData((const char*)pData, (size_t)size, mainID, assistID, 0, pTcpClient->GetTCPSocketInfo().bev, (unsigned int)uIdentification);
    }

    void RegisterTimers(int timerid, int uElapse, pCallBackTimer func)
    {
        TCPClient* pTcpClient = Util::Instance()->GetTCPClient();
        pTcpClient->SetTimer(timerid, uElapse, SERVERTIMER_TYPE_PERISIST);
        pTcpClient->AddTimerCallback(timerid, std::move(std::bind(func)));
    }

    void UnRegisterTimers(int timerid)
    {
        TCPClient* pTcpClient = Util::Instance()->GetTCPClient();
        pTcpClient->KillTimer(timerid);
        pTcpClient->DelTimerCallback(timerid);
    }
}