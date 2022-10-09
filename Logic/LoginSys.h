#pragma once
class PlayerPrepClient;

enum class LoginSysMsgCmd
{
	cs_verification_account		= 1,	// 验证账号密码
	cs_select_server			= 2,	// 选择区服
	cs_select_role				= 3,	// 选角色
	cs_login					= 4,	// 登录
	cs_request_server_list		= 5,    // 请求已有区服
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
	bool NetVerificationAccount(Netmsg& is, PlayerInfo* playerInfo);
	bool NetSelectServer(Netmsg& is, PlayerInfo* playerInfo);
	bool NetSelectRole(Netmsg& is, PlayerInfo* playerInfo);
	bool NetLoginIn(Netmsg& is, PlayerInfo* playerInfo);
	bool NetcRequestServerList(Netmsg& is, PlayerInfo* playerInfo);

private:
	void AddLoginInMap(LoginData& key);
	LoginData* GetLoginInMap(UINT index);

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
