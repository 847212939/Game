#include "pch.h"

namespace MainNameSpace
{
	Util* g_pUtil = Util::Instance();
    pfCallBackEvent g_callbackfunc = NULL;

    void RegisterCallBack(void (*pfCallBackEvent)(REvent eve/*, char m_Source[]*/))
    {
        g_callbackfunc = pfCallBackEvent;
    }

    void __stdcall InitCxxnet()
    {
        Util::InitCxxnet(g_callbackfunc);
    }

}