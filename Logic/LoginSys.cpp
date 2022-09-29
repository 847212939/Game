#include "../Game/stdafx.h"

LoginSys::LoginSys(PlayerPrepClient* ppc)
{
	RegisterNetwk(ppc, LoginSys::Network, MsgCmd::MsgCmd_Login);
}
LoginSys::~LoginSys()
{
}

void LoginSys::Network(PlayerInfo* playerInfo)
{
	if (!playerInfo)
	{
		COUT_LOG(LOG_CERROR, "Player Info is null");
		return;
	}
	if (!playerInfo->pMsg)
	{
		COUT_LOG(LOG_CERROR, "Player Info pMsg is null");
		return;
	}

	LoginSysMsgCmd uAssistantID = (LoginSysMsgCmd)playerInfo->pMsg->netMessageHead.uAssistantID;
	Cis is((char*)playerInfo->pData);

	switch (uAssistantID)
	{
	case LoginSysMsgCmd::cs_verification_account:
	{
		NetVerificationAccount(is, playerInfo);
		break;
	}
	case LoginSysMsgCmd::cs_select_server:
	{
		NetSelectServer(is, playerInfo);
		break;
	}
	case LoginSysMsgCmd::cs_select_role:
	{
		NetSelectRole(is, playerInfo);
		break;
	}
	case LoginSysMsgCmd::cs_login:
	{
		NetLoginIn(is, playerInfo);
		break;
	}
	case LoginSysMsgCmd::cs_request_server_list:
	{
		NetcRequestServerList(is, playerInfo);
		break;
	}
	default:
		break;
	}
}

bool LoginSys::NetVerificationAccount(Cis& is, PlayerInfo* playerInfo)
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
	is >> id >> pw;

	if (id.empty() || pw.empty())
	{
		return false;
	}

	std::string data;
	DPPC->LoadOneSql(id, "useraccount", data);

	LoginData loginData;
	loginData.index = playerInfo->pMsg->uIndex;
	loginData.id = id;
	loginData.pw = pw;

	if (!data.empty())
	{
		uint64_t userid = 0;
		std::string passwaed;

		Cis sqlIs(data);
		sqlIs >> passwaed >> userid;

		if (pw != passwaed)
		{
			// 密码不正确
			return false;
		}
		if (userid <= 0)
		{
			COUT_LOG(LOG_CERROR, "服务器内部错误,请排查错误");
			return false;
		}
		if (DPCC->GetPlayerClientByIndex(loginData.index))
		{
			// 玩家在线
			return false;
		}
		loginData.userId = userid;
	}

	if (loginData.userId <= 0)
	{
		loginData.userId = DUtil->CreateUserId();
	}

	Cos os;
	os << (int)true;
	DTCPC->SendData(playerInfo->pMsg->uIndex, os.str().c_str(), os.str().size(),
		MsgCmd(playerInfo->pMsg->netMessageHead.uMainID),
		playerInfo->pMsg->netMessageHead.uAssistantID, 0,
		playerInfo->pMsg->pBufferevent, 0);

	return true;
}
bool LoginSys::NetSelectServer(Cis& is, PlayerInfo* playerInfo)
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
	is >> serverid;

	if (serverid != BaseCfgMgr.GetServerId())
	{
		return false;
	}
	LoginData* pLoginData = GetLoginInMap(playerInfo->pMsg->uIndex);
	if (!pLoginData)
	{
		return false;
	}

	AddServerIdMap(pLoginData->userId, serverid);

	Cos os;
	os << (int)true;
	DTCPC->SendData(playerInfo->pMsg->uIndex, os.str().c_str(), os.str().size(),
		MsgCmd(playerInfo->pMsg->netMessageHead.uMainID),
		playerInfo->pMsg->netMessageHead.uAssistantID, 0,
		playerInfo->pMsg->pBufferevent, 0);

	return true;
}
bool LoginSys::NetcRequestServerList(Cis& is, PlayerInfo* playerInfo)
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

	LoadServerIds(pLoginData->userId);
	SendServerIds(pLoginData->userId, playerInfo->pMsg);

	return true;
}
bool LoginSys::NetSelectRole(Cis& is, PlayerInfo* playerInfo)
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
	is >> heroid >> netname;

	const CHeroList* pCHeroList = CfgMgr->GetSkillCfg().GetCHeroListCfg(heroid);
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

	Cos os;
	os << (int)true;
	DTCPC->SendData(playerInfo->pMsg->uIndex, os.str().c_str(), os.str().size(),
		MsgCmd(playerInfo->pMsg->netMessageHead.uMainID),
		playerInfo->pMsg->netMessageHead.uAssistantID, 0,
		playerInfo->pMsg->pBufferevent, 0);

	return true;
}
bool LoginSys::NetLoginIn(Cis& is, PlayerInfo* playerInfo)
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
	if (pLoginData->userId <= 0)
	{
		pLoginData->userId = DUtil->CreateUserId();
	}

	DPPC->CreatePlayer(*pLoginData);

	AddLoginInMap(*pLoginData);
	Save(pLoginData->id, pLoginData->pw, pLoginData->userId);
	DelLoginInMap(playerInfo->pMsg->uIndex);

	Cos os;
	os << (int)true;
	DTCPC->SendData(playerInfo->pMsg->uIndex, os.str().c_str(), os.str().size(),
		MsgCmd(playerInfo->pMsg->netMessageHead.uMainID),
		playerInfo->pMsg->netMessageHead.uAssistantID, 0,
		playerInfo->pMsg->pBufferevent, 0);

	return true;
}


void LoginSys::AddLoginInMap(LoginData& key)
{
	m_LoginInMap.insert({ key.index , key });
}
void LoginSys::DelLoginInMap(UINT index)
{
	LoginInMap::iterator it = m_LoginInMap.find(index);
	if (it == m_LoginInMap.end())
	{
		return;
	}

	m_LoginInMap.erase(it);
}
LoginData* LoginSys::GetLoginInMap(UINT index)
{
	LoginInMap::iterator it = m_LoginInMap.find(index);
	if (it == m_LoginInMap.end())
	{
		return nullptr;
	}
	return &it->second;
}

void LoginSys::Save(std::string& id, std::string& pw, uint64_t userid)
{
	Cos os;
	os << pw << userid;
	DPPC->SaveReplaceSQL("useraccount", id, os);
}
void LoginSys::SendServerIds(uint64_t userid, SocketReadLine* pMsg)
{
	if (!pMsg)
	{
		return;
	}
	ServerIdMap::iterator useridIt = m_ServerIdMap.find(userid);
	if (useridIt == m_ServerIdMap.end())
	{
		return;
	}

	Cos os;
	os << (int)useridIt->second.size();

	for (auto it = useridIt->second.begin(); it != useridIt->second.end(); ++it)
	{
		os << *it;
	}

	std::string data = os;

	DTCPC->SendData(pMsg->uIndex, data.c_str(), data.size(),
		MsgCmd(pMsg->netMessageHead.uMainID),
		pMsg->netMessageHead.uAssistantID, 0,
		pMsg->pBufferevent, 0);
}
void LoginSys::LoadServerIds(uint64_t userid)
{
	std::set<int>* serverIdVec = nullptr;
	auto useridIt = m_ServerIdMap.find(userid);
	if (useridIt != m_ServerIdMap.end())
	{
		serverIdVec = &(useridIt->second);
	}
	else
	{
		std::set<int> tmpServerIdVec;
		m_ServerIdMap.insert({ userid , tmpServerIdVec });
		serverIdVec = &(m_ServerIdMap.find(userid)->second);
	}
	if (!serverIdVec)
	{
		return;
	}

	//serverIdVec->clear();

	std::string data;
	DPPC->LoadOneSql("serverlist", userid, data);

	Cis is(data);
	int size = 0;
	is >> size;

	for (int i = 0; i < size; i++)
	{
		int id = 0;
		is >> id;
		serverIdVec->insert(id);
	}
}
void LoginSys::SaveServerIds(uint64_t userid)
{
	auto useridIt = m_ServerIdMap.find(userid);
	if (useridIt == m_ServerIdMap.end())
	{
		return;
	}

	Cos os;
	os << (int)useridIt->second.size();
	for (auto id : useridIt->second)
	{
		os << id;
	}

	std::string str = os;
	DPPC->SaveReplaceSQL("serverlist", userid, str);
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