#include "pch.h"

namespace MainNameSpace
{
	Util* g_pUtil = Util::Instance();

    void __stdcall InitCxxnet()
    {
        Util::InitCxxnet();
    }

}