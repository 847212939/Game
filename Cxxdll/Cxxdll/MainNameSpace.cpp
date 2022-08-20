#include "pch.h"

namespace MainNameSpace
{
    Util* g_pUtil = nullptr;

    void InitCxxnet(pfCallBackEvent pfCallBackEvent)
    {
        Util* g_pUtil = Util::Instance(); 
        g_pUtil->InitCxxnet(pfCallBackEvent);
    }

    void SendData(char* pData, int size, int mainID, int assistID, int uIdentification)
    {
        TCPClient* pTcpClient = g_pUtil->GetTCPClient();
        pTcpClient->SendData((const char*)pData, (size_t)size, (MsgCmd)mainID, assistID, 0, pTcpClient->GetTCPSocketInfo().bev, (unsigned int)uIdentification);
    }

    void RegisterTimers(int timerid, int uElapse, pCallBackTimer func)
    {
        TCPClient* pTcpClient = g_pUtil->GetTCPClient();
        pTcpClient->SetTimer(timerid, uElapse, SERVERTIMER_TYPE_PERISIST);
        pTcpClient->AddTimerCallback(timerid, std::move(std::bind(func)));
    }
}