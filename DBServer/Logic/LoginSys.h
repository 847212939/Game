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
	void DelLoginInMap(unsigned int index);

private:
	void Network(PlayerInfo* playerInfo);

private:
	bool NetVerificationAccount(Netmsg& msg, PlayerInfo* playerInfo);
	bool NetSelectServer(Netmsg& msg, PlayerInfo* playerInfo);
	bool NetSelectRole(Netmsg& msg, PlayerInfo* playerInfo);
	bool NetLoginIn(Netmsg& msg, PlayerInfo* playerInfo);
	bool NetcRequestServerList(Netmsg& msg, PlayerInfo* playerInfo);

private:
	void AddLoginInMap(LoginData& key);
	LoginData* GetLoginInMap(unsigned int index);

	void AddServerIdMap(uint64_t userid, int serverId);

private:
	void LoadServerIds(uint64_t userid);
	void SaveServerIds(uint64_t userid);
	void SendServerIds(uint64_t userid, SocketReadLine* pMsg);
	void SaveUserAccount(std::string& id, std::string& pw, uint64_t userid = 0);

private:
	MapLoginIn m_LoginInMap;
	MapServerId m_ServerIdMap;
};
