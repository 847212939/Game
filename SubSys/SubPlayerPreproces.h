#pragma once

class SubPlayerPreproces : public PlayerPreproces
{
public:
	virtual~SubPlayerPreproces();
	SubPlayerPreproces(TCPClient* pTCPClient);

public:
	LoginSys& GetLoginSys();

private:
	LoginSys m_LoginSys;
};
