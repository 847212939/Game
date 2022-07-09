#pragma once

class SubPlayerPreproces : public PlayerPreproces
{
public:
	~SubPlayerPreproces();
	SubPlayerPreproces(TCPClient* pTCPClient);

private:
	void Register();
	void CreateTable(std::string name);

private:
	LoginSys m_LoginSys;
};
