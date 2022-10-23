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
	Netmsg msg((char*)playerInfo->pData);

	switch (uAssistantID)
	{
	case LoginSysMsgCmd::cs_load:
	{
		LoadLoginMysql(msg, playerInfo);
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

	std::string idPw = id + "\n" + pw + "\n" + std::to_string(playerInfo->pMsg->uIndex);
	DPPC->GetMysqlClient().LoadLoginMysql(idPw, SLoadMysql("useraccount", MsgCmd::MsgCmd_Login,
		(unsigned int)LoginSysMsgCmd::cs_load, (unsigned int)MsgCmd::MsgCmd_PlayerPreproces));
	
	return true;
}
bool LoginSys::LoadLoginMysql(Netmsg& msg, PlayerInfo* playerInfo)
{
	int index = 0;
	std::string id, pw;
	msg >> id >> pw >> index;

	LoginData loginData;
	loginData.index = index;
	loginData.id = id;
	loginData.pw = pw;

	if (!msg.empty())
	{
		uint64_t userid = 0;
		std::string passwaed;

		msg >> passwaed >> userid;

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

	AddLoginInMap(loginData);
	TCPSocketInfo* sockInfo = DTCPC->GetTCPSocketInfo(index);
	if (sockInfo)
	{
		Netmsg msg;
		msg << (int)true;
		DTCPC->SendMsg(index, msg.str().c_str(), msg.str().size(),
			MsgCmd::MsgCmd_Login, (int)LoginSysMsgCmd::cs_verification_account, 0, sockInfo->bev, 0);
	}

	DPPC->SendOperateResults(playerInfo->pMsg);

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

	if (serverid != BaseCfgMgr.GetServerId())
	{
		return false;
	}
	LoginData* pLoginData = GetLoginInMap(playerInfo->pMsg->uIndex);
	if (!pLoginData)
	{
		return false;
	}

	pLoginData->serverId = serverid;

	DPPC->SendOperateResults(playerInfo->pMsg);

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

	LoadServerIds(pLoginData->userId);
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

	DPPC->SendOperateResults(playerInfo->pMsg);

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

	DPPC->CreatePlayer(*pLoginData);
	DPPC->SendOperateResults(playerInfo->pMsg);

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

	std::string data;
	DPPC->LoadOneSql("serverlist", userid, data);

	Netmsg msg(data);
	int size = 0;
	msg >> size;

	for (int i = 0; i < size; i++)
	{
		int id = 0;
		msg >> id;
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

	Netmsg msg;
	msg << (int)useridIt->second.size();
	for (auto id : useridIt->second)
	{
		msg << id;
	}

	std::string str = msg;
	DPPC->SaveReplaceSQL("serverlist", userid, str);
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
	DTCPC->SendMsg(pMsg->uIndex, data.c_str(), data.size(),
		MsgCmd(pMsg->netMessageHead.uMainID),
		pMsg->netMessageHead.uAssistantID, 0,
		pMsg->pBufferevent, 0);
}
void LoginSys::SaveUserAccount(std::string& id, std::string& pw, uint64_t userid)
{
	Netmsg msg;
	msg << pw << userid;
	DPPC->SaveReplaceSQL("useraccount", id, msg);
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