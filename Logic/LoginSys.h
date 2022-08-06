#pragma once

class PlayerPrepClient;
class LoginSys
{
public:
	enum class LoginSysMsgCmd
	{
		CS_Login = 1,  // µÇÂ¼
	};

public:
	LoginSys(PlayerPrepClient* playerPrepClient);
	virtual~LoginSys();

public:
	bool LoginIn(std::string& id, std::string& passwaed, uint64_t& userId);
	void TimerCallback();

private:
	void Register();
	void NetworkCallback(PlayerInfo* playerInfo);

private:
	bool LoginIn(CIstringstream& is, PlayerInfo* playerInfo);

private:
	PlayerPrepClient* m_PlayerPrepClient;
};
