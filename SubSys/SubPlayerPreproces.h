#pragma once

class SubPlayerPreproces : public PlayerPreproces
{
public:
	~SubPlayerPreproces();
	SubPlayerPreproces(TCPClient* pTCPClient);

public:
	LoginSys& GetLoginSys();

private:
	void Register();

private:
	LoginSys m_LoginSys;
};
