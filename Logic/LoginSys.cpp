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

// �������
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

		Cis is(data);
		is >> passwaed >> userid;

		if (pw != passwaed)
		{
			// ���벻��ȷ
			return false;
		}
		if (userid <= 0)
		{
			COUT_LOG(LOG_CERROR, "�������ڲ�����,���Ų����");
			return false;
		}
		if (DPCC->GetPlayerClientByIndex(loginData.index))
		{
			// �������
			return false;
		}
		loginData.userId = userid;
	}

	AddLoginInMap(loginData);


	Cos os;
	os << (int)true;
	DTCPC->SendData(playerInfo->pMsg->uIndex, os.str().c_str(), os.str().size(),
		MsgCmd(playerInfo->pMsg->netMessageHead.uMainID),
		playerInfo->pMsg->netMessageHead.uAssistantID, 0,
		playerInfo->pMsg->pBufferevent, 0);

	return true;
}

// ѡ�������
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

	if (serverid <= 0)
	{
		DelLoginInMap(playerInfo->pMsg->uIndex);
		return false;
	}
	if (serverid != BaseCfgMgr.GetServerId())
	{
		DelLoginInMap(playerInfo->pMsg->uIndex);
		return false;
	}
	LoginData* pLoginData = GetLoginInMap(playerInfo->pMsg->uIndex);
	if (!pLoginData)
	{
		DelLoginInMap(playerInfo->pMsg->uIndex);
		return false;
	}

	{
		Cos os;
		os << (int)true;
		DTCPC->SendData(playerInfo->pMsg->uIndex, os.str().c_str(), os.str().size(),
			MsgCmd(playerInfo->pMsg->netMessageHead.uMainID),
			playerInfo->pMsg->netMessageHead.uAssistantID, 0,
			playerInfo->pMsg->pBufferevent, 0);
	}

	{
		Cos os;
		os << serverid;
		std::string str = os;
		DPPC->SaveReplaceSQL("serverlist", pLoginData->userId, str);
		return true;
	}
}

// ѡ��
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
		DelLoginInMap(playerInfo->pMsg->uIndex);
		return false;
	}

	LoginData* pLoginData = GetLoginInMap(playerInfo->pMsg->uIndex);
	if (!pLoginData)
	{
		DelLoginInMap(playerInfo->pMsg->uIndex);
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
		DelLoginInMap(playerInfo->pMsg->uIndex);
		return false;
	}
	if (pLoginData->userId <= 0)
	{
		pLoginData->userId = DUtil->CreateUserId();
	}

	DPPC->CreatePlayer(*pLoginData);

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

	std::string data;
	DPPC->LoadOneSql("serverlist", pLoginData->userId, data);

	Cos os;
	os << data;
	DTCPC->SendData(playerInfo->pMsg->uIndex, os.str().c_str(), os.str().size(),
		MsgCmd(playerInfo->pMsg->netMessageHead.uMainID),
		playerInfo->pMsg->netMessageHead.uAssistantID, 0,
		playerInfo->pMsg->pBufferevent, 0);

	return true;
}

void LoginSys::AddLoginInMap(LoginData key)
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
