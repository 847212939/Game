#pragma once

class PlayerPrepClient;
class LoginSys
{
public:
	enum class LoginSysMsgCmd
	{
		CS_Login = 1,  // ��¼
	};

public:
	LoginSys(PlayerPrepClient* pSubPlayerPreproces);
	virtual~LoginSys();

public:
	bool LoginIn(std::string& id, std::string& passwaed, uint64_t& userId);
	void TimerCallback();

private:
	void Register();
	void NetworkCallback(PlayerInfo* pPlayerInfo);

private:
	bool LoginIn(CIstringstream& is, PlayerInfo* pPlayerInfo);

private:
	PlayerPrepClient* m_pSubPlayerPreproces;
};
