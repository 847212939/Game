#include "../Game/stdafx.h"

SubPlayerPreproces::SubPlayerPreproces(TCPClient* pTCPClient) :
	PlayerPreproces(pTCPClient),
	m_LoginSys(this)
{
	RegisterCreat(this, "move");
	RegisterCreatS(this, "useraccount");
}

SubPlayerPreproces::~SubPlayerPreproces()
{
}

LoginSys& SubPlayerPreproces::GetLoginSys()
{
	return m_LoginSys;
}