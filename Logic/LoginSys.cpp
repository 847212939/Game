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

	LoginSysMsgCmd uAssistantID = 
		(LoginSysMsgCmd)playerInfo->pMsg->netMessageHead.uAssistantID;
	Cis is((char*)playerInfo->pData);

	switch (uAssistantID)
	{
	case LoginSysMsgCmd::cs_verification_account:
	{
		VerificationAccount(is, playerInfo);
		break;
	}
	case LoginSysMsgCmd::cs_select_role:
	{
		SelectRole(is, playerInfo);
		break;
	}
	case LoginSysMsgCmd::cs_login:
	{
		LoginIn(is, playerInfo);
		break;
	}
	default:
		break;
	}
}

// 检查密码
bool LoginSys::VerificationAccount(Cis& is, PlayerInfo* playerInfo)
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
		std::string passwaed;

		Cis is(data);
		is >> passwaed;

		if (pw != passwaed)
		{
			// 密码不正确
			return false;
		}

		// 检查玩家是否在线
		uint64_t userid = 0;
		is >> userid;

		if (userid <= 0)
		{
			return false;
		}
		if (DPCC->GetPlayerClientByUserid(userid))
		{
			// 玩家在线
			return false;
		}
		loginData.userId = userid;
	}

	AddLoginInMap(loginData);

	DTCPC->SendData(loginData.index, nullptr, 0, MsgCmd::MsgCmd_Login, 
		(int)LoginSysMsgCmd::cs_verification_account, 0, 
		playerInfo->pMsg->pBufferevent, 0);

	return true;
}

// 选角
bool LoginSys::SelectRole(Cis& is, PlayerInfo* playerInfo)
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

	DTCPC->SendData(pLoginData->index, nullptr, 0, MsgCmd::MsgCmd_Login,
		(int)LoginSysMsgCmd::cs_verification_account, 
		0, playerInfo->pMsg->pBufferevent, 0);

	return true;
}

bool LoginSys::LoginIn(Cis& is, PlayerInfo* playerInfo)
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
