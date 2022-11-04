#include "../stdafx.h"

LoginSys::LoginSys(PlayerPrepClient* ppc)
{
	RegisterNetwk(ppc, LoginSys::Network, MsgCmd::MsgCmd_Login);
}
LoginSys::~LoginSys()
{
}

void LoginSys::Network(PlayerInfo* playerInfo)
{
	if (G_NetClient->GetServerType() == 
		ServiceType::SERVICE_TYPE_CROSS)
	{
		return;
	}
	if (!playerInfo)
	{
		return;
	}
	if (!playerInfo->pMsg)
	{
		return;
	}

	LoginSysMsgCmd uAssistantID = (LoginSysMsgCmd)playerInfo->pMsg->netMessageHead.uAssistantID;
	Netmsg msg((char*)playerInfo->pData, playerInfo->pMsg->uHandleSize, 0);

	switch (uAssistantID)
	{
	case LoginSysMsgCmd::cs_load:
	{
		LoadLoginMysql(msg, playerInfo);
		break;
	}
	case LoginSysMsgCmd::cs_load_server_list:
	{
		LoadServerListMysql(msg, playerInfo);
		break;
	}
	case LoginSysMsgCmd::cs_verification_account:
	{
		NetVerificationAccount(msg, playerInfo);
		break;
	}
	case LoginSysMsgCmd::cs_select_server:
	{
		NetSelectServer(msg, playerInfo);
		break;
	}
	case LoginSysMsgCmd::cs_select_role:
	{
		NetSelectRole(msg, playerInfo);
		break;
	}
	case LoginSysMsgCmd::cs_login:
	{
		NetLoginIn(msg, playerInfo);
		break;
	}
	case LoginSysMsgCmd::cs_request_server_list:
	{
		NetcRequestServerList(msg, playerInfo);
		break;
	}
	default:
		break;
	}
}

bool LoginSys::NetVerificationAccount(Netmsg& msg, PlayerInfo* playerInfo)
{
	if (!playerInfo)
	{
		return false;
	}
	if (!playerInfo->pMsg)
	{
		return false;
	}

	std::string id, pw;
	msg >> id >> pw;

	if (id.empty() || pw.empty())
	{
		return false;
	}

	std::string idPw = id + "\n" + pw;
	MysqlClient::LoadLoginMysql(idPw, SLoadMysql(playerInfo->pMsg->uIndex, "useraccount", MsgCmd::MsgCmd_Login,
		(unsigned int)LoginSysMsgCmd::cs_load, MsgCmd::MsgCmd_PlayerPreproces));
	
	return true;
}
bool LoginSys::LoadServerListMysql(Netmsg& msg, PlayerInfo* playerInfo)
{
	int serverid = 0;
	unsigned int uIndex = 0;
	msg >> serverid
		>> uIndex;

	LoginData* pLoginData = GetLoginInMap(uIndex);
	if (!pLoginData)
	{
		return false;
	}
	std::set<int>* serverIdVec = nullptr;
	auto useridIt = m_ServerIdMap.find(pLoginData->userId);
	if (useridIt != m_ServerIdMap.end())
	{
		serverIdVec = &(useridIt->second);
	}
	else
	{
		std::set<int> tmpServerIdVec;
		m_ServerIdMap.insert({ pLoginData->userId , tmpServerIdVec });
		serverIdVec = &(m_ServerIdMap.find(pLoginData->userId)->second);
	}
	if (!serverIdVec)
	{
		return false;
	}

	int size = 0;
	msg >> size;

	for (int i = 0; i < size; i++)
	{
		int id = 0;
		msg >> id;
		serverIdVec->insert(id);
	}

	return true;
}
bool LoginSys::LoadLoginMysql(Netmsg& msg, PlayerInfo* playerInfo)
{
	if (G_NetClient->GetServerType() == ServiceType::SERVICE_TYPE_CROSS)
	{
		return false;
	}
	int serverid = 0;
	unsigned int uIndex = 0;
	std::string account;
	std::string password;
	msg >> serverid
		>> account
		>> password
		>> uIndex;

	LoginData loginData;
	loginData.index = uIndex;
	loginData.id = account;
	loginData.pw = password;

	if (!msg.empty())
	{
		uint64_t userid = 0;
		std::string sqlPasswaed;

		msg >> sqlPasswaed >> userid;

		if (password != sqlPasswaed)
		{
			// 密码不正确
			return false;
		}
		if (userid <= 0)
		{
			return false;
		}
		if (G_PlayerCenterClient->GetPlayerByIndex(loginData.index))
		{
			// 玩家在线
			return false;
		}
		loginData.userId = userid;
	}
	if (loginData.userId <= 0)
	{
		loginData.userId = G_Util->CreateUserId();
	}

	AddLoginInMap(loginData);
	TCPSocketInfo* sockInfo = G_NetClient->GetTCPSocketInfo((int)uIndex);
	if (sockInfo)
	{
		Netmsg msg;
		msg << (int)true;
		G_NetClient->SendMsg((int)uIndex, msg.str().c_str(), msg.str().size(),
			MsgCmd::MsgCmd_Login, (int)LoginSysMsgCmd::cs_verification_account, 0, sockInfo->bev, 0);
	}

	G_PlayerPrepClient->SendOperateResults(playerInfo->pMsg);

	return true;
}
bool LoginSys::NetSelectServer(Netmsg& msg, PlayerInfo* playerInfo)
{
	if (!playerInfo)
	{
		return false;
	}
	if (!playerInfo->pMsg)
	{
		return false;
	}

	int serverid = 0;
	msg >> serverid;

	if (serverid != G_CfgMgr->GetCBaseCfgMgr().GetServerId())
	{
		return false;
	}
	LoginData* pLoginData = GetLoginInMap(playerInfo->pMsg->uIndex);
	if (!pLoginData)
	{
		return false;
	}

	pLoginData->serverId = serverid;

	G_PlayerPrepClient->SendOperateResults(playerInfo->pMsg);

	return true;
}
bool LoginSys::NetcRequestServerList(Netmsg& msg, PlayerInfo* playerInfo)
{
	if (!playerInfo)
	{
		return false;
	}
	if (!playerInfo->pMsg)
	{
		return false;
	}
	LoginData* pLoginData = GetLoginInMap(playerInfo->pMsg->uIndex);
	if (!pLoginData)
	{
		return false;
	}

	LoadServerIds(playerInfo, pLoginData->userId);
	SendServerIds(pLoginData->userId, playerInfo->pMsg);

	return true;
}
bool LoginSys::NetSelectRole(Netmsg& msg, PlayerInfo* playerInfo)
{
	if (!playerInfo)
	{
		return false;
	}
	if (!playerInfo->pMsg)
	{
		return false;
	}
	
	int heroid = 0;
	std::string netname;
	msg >> heroid >> netname;

	const CHeroList* pCHeroList = G_CfgMgr->GetSkillCfg().GetCHeroListCfg(heroid);
	if (!pCHeroList)
	{
		return false;
	}

	LoginData* pLoginData = GetLoginInMap(playerInfo->pMsg->uIndex);
	if (!pLoginData)
	{
		return false;
	}

	pLoginData->roleid = heroid;
	pLoginData->netName = netname;
	pLoginData->roleType = pCHeroList->heroType;
	pLoginData->roleName = pCHeroList->heroName;

	G_PlayerPrepClient->SendOperateResults(playerInfo->pMsg);

	return true;
}
bool LoginSys::NetLoginIn(Netmsg& msg, PlayerInfo* playerInfo)
{
	if (!playerInfo)
	{
		return false;
	}
	if (!playerInfo->pMsg)
	{
		return false;
	}
	LoginData* pLoginData = GetLoginInMap(playerInfo->pMsg->uIndex);
	if (!pLoginData)
	{
		return false;
	}

	G_PlayerPrepClient->CreatePlayer(*pLoginData);
	G_PlayerPrepClient->SendOperateResults(playerInfo->pMsg);

	AddServerIdMap(pLoginData->userId, pLoginData->serverId);
	SaveServerIds(pLoginData->userId);
	SaveUserAccount(pLoginData->id, pLoginData->pw, pLoginData->userId);
	DelLoginInMap(playerInfo->pMsg->uIndex);

	return true;
}

void LoginSys::DelLoginInMap(unsigned int index)
{
	MapLoginIn::iterator it = m_LoginInMap.find(index);
	if (it == m_LoginInMap.end())
	{
		return;
	}

	m_LoginInMap.erase(it);
}
void LoginSys::AddLoginInMap(LoginData& key)
{
	m_LoginInMap.insert({ key.index , key });
}
LoginData* LoginSys::GetLoginInMap(unsigned int index)
{
	MapLoginIn::iterator it = m_LoginInMap.find(index);
	if (it == m_LoginInMap.end())
	{
		return nullptr;
	}
	return &it->second;
}

void LoginSys::LoadServerIds(PlayerInfo* playerInfo, uint64_t userid)
{
	SLoadMysql loadMysql(playerInfo->pMsg->uIndex, "serverlist", MsgCmd::MsgCmd_Login,
		(unsigned int)LoginSysMsgCmd::cs_load_server_list, 
		MsgCmd::MsgCmd_PlayerPreproces);

	MysqlClient::LoadPlayerMysql(userid, loadMysql);
}
void LoginSys::SaveServerIds(uint64_t userid)
{
	auto useridIt = m_ServerIdMap.find(userid);
	if (useridIt == m_ServerIdMap.end())
	{
		return;
	}

	Netmsg msg;
	msg << (int)useridIt->second.size();
	for (auto id : useridIt->second)
	{
		msg << id;
	}

	MysqlClient::SaveReplacePlayerMysql(userid, "serverlist", msg.str());
}
void LoginSys::SendServerIds(uint64_t userid, SocketReadLine* pMsg)
{
	MapServerId::iterator useridIt = m_ServerIdMap.find(userid);
	if (!pMsg || useridIt == m_ServerIdMap.end())
	{
		return;
	}

	Netmsg msg;
	msg << (int)useridIt->second.size();
	for (auto it = useridIt->second.begin(); it != useridIt->second.end(); ++it)
	{
		msg << *it;
	}

	std::string data = msg;
	G_NetClient->SendMsg(pMsg->uIndex, data.c_str(), data.size(),
		MsgCmd(pMsg->netMessageHead.uMainID),
		pMsg->netMessageHead.uAssistantID, 0,
		pMsg->pBufferevent, 0);
}
void LoginSys::SaveUserAccount(std::string& id, std::string& pw, uint64_t userid)
{
	Netmsg msg;
	msg << pw << userid;

	MysqlClient::SaveReplaceLoginMysql(id, "useraccount", msg.str());
}

void LoginSys::AddServerIdMap(uint64_t userid, int serverId)
{
	auto useridIt = m_ServerIdMap.find(userid);
	if (useridIt == m_ServerIdMap.end())
	{
		std::set<int> serveridVec;
		serveridVec.insert(serverId);
		m_ServerIdMap.insert({ userid , serveridVec });
	}
	else
	{
		useridIt->second.insert(serverId);
	}
}