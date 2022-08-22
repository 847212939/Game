#pragma once
class PlayerPrepClient;

enum class LoginSysMsgCmd
{
	cs_verification_account		= 1,	// ��֤�˺�����
	cs_select_server			= 2,	// ѡ������
	cs_select_role				= 3,	// ѡ��ɫ
	cs_login					= 4,	// ��¼
};

class LoginSys
{
public:
	LoginSys(PlayerPrepClient* ppc);
	virtual~LoginSys();

public:
	void DelLoginInMap(UINT index);

private:
	void Network(PlayerInfo* playerInfo);

private:
	bool VerificationAccount(Cis& is, PlayerInfo* playerInfo);
	bool SelectServer(Cis& is, PlayerInfo* playerInfo);
	bool SelectRole(Cis& is, PlayerInfo* playerInfo);
	bool LoginIn(Cis& is, PlayerInfo* playerInfo);

private:
	void AddLoginInMap(LoginData key);
	LoginData* GetLoginInMap(UINT index);

private:
	void Save(std::string& id, std::string& pw, uint64_t userid = 0);

private:
	LoginInMap m_LoginInMap;
};
