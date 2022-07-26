#include "../Game/stdafx.h"

PlayerPrepClient::PlayerPrepClient(TCPClient* pTCPClient) :
	PlayerPrep(pTCPClient),
	m_LoginSys(this)
{
	RegisterCreat(this, "move");
	RegisterCreatS(this, "useraccount");
}

PlayerPrepClient::~PlayerPrepClient()
{
}

LoginSys& PlayerPrepClient::GetLoginSys()
{
	return m_LoginSys;
}