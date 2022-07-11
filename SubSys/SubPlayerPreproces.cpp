#include "../Game/stdafx.h"

SubPlayerPreproces::SubPlayerPreproces(TCPClient* pTCPClient) :
	PlayerPreproces(pTCPClient),
	m_LoginSys(this)
{
	Register();
}

SubPlayerPreproces::~SubPlayerPreproces()
{
}

// 创建数据库
void SubPlayerPreproces::Register()
{
	RegisterCreat(this, "move");
	RegisterCreat(this, "useraccount");
}

LoginSys& SubPlayerPreproces::GetLoginSys()
{
	return m_LoginSys;
}