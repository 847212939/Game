#pragma once

class LoginSys
{
public:
	// ×¢²áµÇÂ¼×ÓÐ­Òé
	enum class LoginSysMsgCmd
	{
		CS_Login = 1,  // µÇÂ¼
	};
public:
	LoginSys(SubPlayerPreproces* pSubPlayerPreproces);
	~LoginSys();

private:
	void CallBackFunInit();

private:
	// µÇÂ¼
	void LoginInAccount(PlayerInfo* pPlayerInfo);
	bool LoginIn(CIstringstream& is, PlayerInfo* pPlayerInfo);
	bool LoginIn(std::string& id, std::string& passwaed, PlayerInfo* pPlayerInfo);

private:
	std::string LoadUserId(std::string& id);
	std::string LoadUserAccount(std::string& id);

private:
	SubPlayerPreproces* m_pSubPlayerPreproces;
};
