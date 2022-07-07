#pragma once

class LoginSys
{
public:
	// ע���¼��Э��
	enum class LoginSysMsgCmd
	{
		CS_Login = 1,  // ��¼
	};
public:
	LoginSys(SubPlayerPreproces* pSubPlayerPreproces);
	~LoginSys();

private:
	void CallBackFunInit();

private:
	// ��¼
	void LoginInAccount(PlayerInfo* pPlayerInfo);
	bool LoginIn(CIstringstream& is, PlayerInfo* pPlayerInfo);
	bool LoginIn(std::string& id, std::string& passwaed, PlayerInfo* pPlayerInfo);

private:
	std::string LoadUserId(std::string& id);
	std::string LoadUserAccount(std::string& id);

private:
	SubPlayerPreproces* m_pSubPlayerPreproces;
};
