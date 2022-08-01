#pragma once

class PlayerPrepClient;
class LoginSys
{
public:
	enum class LoginSysMsgCmd
	{
		cs_login = 1,  // µÇÂ¼
	};

public:
	LoginSys(PlayerPrepClient* ppc);
	virtual~LoginSys();

public:
	bool LoginIn(std::string& id, std::string& passwaed, uint64_t& userId);
	void TimerCallback();

private:
	void NetworkCallback(PlayerInfo* playerInfo);

private:
	bool LoginIn(Cis& is, PlayerInfo* playerInfo);
};
