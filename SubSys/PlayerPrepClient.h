#pragma once

class PlayerPrepClient : public PlayerPreproces
{
public:
	virtual~PlayerPrepClient();
	PlayerPrepClient(TCPClient* pTCPClient);

public:
	LoginSys& GetLoginSys();

private:
	LoginSys m_LoginSys;
};
