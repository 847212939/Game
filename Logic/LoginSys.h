#pragma once

class SubPlayerPreproces;
class LoginSys
{
public:
	enum class LoginSysMsgCmd
	{
		CS_Login = 1,  // µÇÂ¼
	};
public:
	LoginSys(SubPlayerPreproces* pSubPlayerPreproces);
	~LoginSys();

private:
	void Register();
	void NetworkCallback(PlayerInfo* pPlayerInfo);

private:
	bool LoginIn(CIstringstream& is, PlayerInfo* pPlayerInfo);
	bool LoginIn(std::string& id, std::string& passwaed, PlayerInfo* pPlayerInfo);

private:
	SubPlayerPreproces* m_pSubPlayerPreproces;
};
