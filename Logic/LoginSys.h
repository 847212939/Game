#pragma once
class PlayerPrepClient;

enum class LoginSysMsgCmd
{
	cs_verification_account		= 1,	// 验证账号密码
	cs_select_role				= 2,	// 选角色
	cs_login					= 3,	// 登录
};

class LoginSys
{
public:
	LoginSys(PlayerPrepClient* ppc);
	virtual~LoginSys();

private:
	void Network(PlayerInfo* playerInfo);

private:
	bool VerificationAccount(Cis& is, PlayerInfo* playerInfo);
	bool SelectRole(Cis& is, PlayerInfo* playerInfo);
	bool LoginIn(Cis& is, PlayerInfo* playerInfo);

private:
	void DelLoginInMap(UINT index);
	void AddLoginInMap(LoginData key);
	LoginData* GetLoginInMap(UINT index);

private:
	void Save(std::string& id, std::string& pw, uint64_t userid = 0);

private:
	LoginInMap m_LoginInMap;
};
