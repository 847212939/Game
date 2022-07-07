#include "../Game/stdafx.h"

SubPlayerPreproces::SubPlayerPreproces(TCPClient* pTCPClient) :
	PlayerPreproces(pTCPClient),
	m_LoginSys(this)
{
	CallBackFunInit();
}

SubPlayerPreproces::~SubPlayerPreproces()
{
}

// 初始化消息回调函数
void SubPlayerPreproces::CallBackFunInit()
{
}
