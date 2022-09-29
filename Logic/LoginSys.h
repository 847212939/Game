#pragma once
class PlayerPrepClient;

enum class LoginSysMsgCmd
{
	cs_verification_account		= 1,	// ��֤�˺�����
	cs_select_server			= 2,	// ѡ������
	cs_select_role				= 3,	// ѡ��ɫ
	cs_login					= 4,	// ��¼
	cs_request_server_list		= 5,    // ������������
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
	bool NetVerificationAccount(Cis& is, PlayerInfo* playerInfo);
	bool NetSelectServer(Cis& is, PlayerInfo* playerInfo);
	bool NetSelectRole(Cis& is, PlayerInfo* playerInfo);
	bool NetLoginIn(Cis& is, PlayerInfo* playerInfo);
	bool NetcRequestServerList(Cis& is, PlayerInfo* playerInfo);

private:
	void AddLoginInMap(LoginData key);
	LoginData* GetLoginInMap(UINT index);

	void AddServerIdMap(uint64_t userid, int serverId);

private:
	void Save(std::string& id, std::string& pw, uint64_t userid = 0);
	void LoadServerIds(uint64_t userid);
	void SaveServerIds(uint64_t userid);
	void SendServerIds(uint64_t userid, SocketReadLine* pMsg);

private:
	LoginInMap m_LoginInMap;
	std::map<uint64_t, std::set<int>> m_ServerIdMap;
};
