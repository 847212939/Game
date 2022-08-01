#pragma once

class PlayerPrepClient : public PlayerPrep
{
public:
	PlayerPrepClient();
	virtual~PlayerPrepClient();

public:
	LoginSys& GetLoginSys();

private:
	LoginSys m_LoginSys;
};
