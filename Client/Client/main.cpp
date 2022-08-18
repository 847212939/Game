#define WIN32_LEAN_AND_MEAN

#include "stdafx.h"

void HandlerPlayerThread(Net* g_mgr)
{
    // 链接
    g_mgr->Connect();
    NetMsg::RecvData(g_mgr);

    // 登录
    g_mgr->GetLoginSys().VerificationAccount(g_mgr);
    g_mgr->GetLoginSys().SelectRole(g_mgr);
    g_mgr->GetLoginSys().LoginIn(g_mgr);

    std::cout << "上线" << std::endl;
}

int main()
{
    std::vector<std::thread*> vec;
    std::vector< Net*> Netvec;

    for (int i = 0; i < 1; i++)
    {
        Netvec.push_back(new Net);
    }
    for (int i = 0; i < 1; i++)
    {
        vec.push_back(new std::thread(HandlerPlayerThread, Netvec[i]));

        if (i == 7999)
        {
            return 0;
        }
    }

    for (auto& th : vec)
    {
        th->join();
    }

    return 0;
}