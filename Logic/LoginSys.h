#pragma once

enum class LoginSysMsgCmd
{
	cs_login = 1,  // ��¼
};

class PlayerPrepClient;
class LoginSys
{
public:
	LoginSys(PlayerPrepClient* ppc);
	virtual~LoginSys();

public:
	bool LoginIn(std::string& id, std::string& passwaed, uint64_t& userId);

private:
	void Network(PlayerInfo* playerInfo);

private:
	bool LoginIn(Cis& is, PlayerInfo* playerInfo);
};
