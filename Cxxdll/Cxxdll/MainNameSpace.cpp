#include "pch.h"

namespace MainNameSpace
{
	Util* g_pUtil = Util::Instance();
    pfCallBackEvent g_callbackfunc = NULL;

    void RegisterCallBack(void (*pfCallBackEvent)(REvent eve/*, char m_Source[]*/))
    {
        g_callbackfunc = pfCallBackEvent;
    }

    void InitCxxnet()
    {
        Util::InitCxxnet(g_callbackfunc);
    }

    void SendData(char* pData, int size, int mainID, int assistID, int uIdentification)
    {
        TCPClient* pTcpClient = g_pUtil->GetTCPClient();
        pTcpClient->SendData((const char*)pData, (size_t)size, (MsgCmd)mainID, assistID, 0, pTcpClient->GetTCPSocketInfo().bev, (unsigned int)uIdentification);
    }
}