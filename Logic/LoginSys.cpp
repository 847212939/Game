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
	LoginSysMsgCmd uAssistantID = (LoginSysMsgCmd)playerInfo->pMsg->netMessageHead.uAssistantID;
	Cis is((char*)playerInfo->pData);

	switch (uAssistantID)
	{
	case LoginSysMsgCmd::cs_login:
	{
		LoginIn(is, playerInfo);
		break;
	}
	default:
		break;
	}
}

bool LoginSys::LoginIn(Cis& is, PlayerInfo* playerInfo)
{
	std::string id, pw;
	is >> id >> pw;

	DPPC->CreatePlayer(playerInfo->pMsg->uIndex, id, pw);

	return true;
}

bool LoginSys::LoginIn(std::string& id, std::string& passwaed, uint64_t& userId)
{
	if (id.empty() || passwaed.empty())
	{
		// �˺Ų�����
		COUT_LOG(LOG_CINFO, "id or passworld is empty");
		return false;
	}

	// ���ݿ��ѯ
	std::string data;
	DPPC->LoadOneSql(id, "useraccount", data);

	if (data.empty())
	{
		userId = DUtil->CreateUserId();

		Cos os;
		os << passwaed << userId;

		DPPC->SaveReplaceSQL("useraccount", id, os);

		return true;
	}
	else
	{
		std::string pw;
		Cis is(data);
		is >> pw >> userId;
		
		if (pw != passwaed)
		{
			// ���벻��ȷ
			COUT_LOG(LOG_CINFO, "passworld is error");
			return false;
		}
		else
		{
			if (userId == 0)
			{
				COUT_LOG(LOG_CERROR, "�˻������쳣");
				return false;
			}
		}
	}

	return true;
}

