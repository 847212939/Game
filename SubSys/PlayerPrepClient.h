#pragma once

class PlayerPrepClient : public PlayerPrep
{
public:
	virtual~PlayerPrepClient();
	PlayerPrepClient(TCPClient* pTCPClient);

public:
	LoginSys& GetLoginSys();

private:
	LoginSys m_LoginSys;
};
