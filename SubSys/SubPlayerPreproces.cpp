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

void SubPlayerPreproces::Register()
{
	RegisterCreat(this, "move");
	RegisterCreatS(this, "useraccount");
}

LoginSys& SubPlayerPreproces::GetLoginSys()
{
	return m_LoginSys;
}