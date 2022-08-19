#include "pch.h"

namespace NTCPClient
{
	TCPClient* g_pObj = TCPClient::Instance();

    int __stdcall Add(int n1, int n2)
    {
        return g_pObj->Add(n1, n2);
    }

    int __stdcall Sub(int n1, int n2)
    {
        return g_pObj->Sub(n1, n2);
    }
}