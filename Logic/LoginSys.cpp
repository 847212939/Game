#include "../Game/stdafx.h"

LoginSys::LoginSys(PlayerPrepClient* ppc)
{
	RegisterNetwk(ppc, this, LoginSys::NetworkCallback, MsgCmd::MsgCmd_Login);
	RegisterTimer(ppc, this, LoginSys::TimerCallback, TimerCmd::TimerCmd_Test, 1000, SERVERTIMER_TYPE_PERISIST);
}

LoginSys::~LoginSys()
{
}

void LoginSys::TimerCallback()
{
	COUT_LOG(LOG_CINFO, "��ʱ���ص�����");
	UnRegisterTimer(DPlayerPrepClient, TimerCmd::TimerCmd_Test);
}

void LoginSys::NetworkCallback(PlayerInfo* playerInfo)
{
	if (!playerInfo)
	{
		COUT_LOG(LOG_CERROR, "Player Info is null");
		return;
	}
	if (!playerInfo->m_pMsg || !playerInfo->m_pTcpSockInfo)
	{
		COUT_LOG(LOG_CERROR, "player info pMsg is null or player info sock info is null");
		return;
	}
	if (!playerInfo->m_pTcpSockInfo->isConnect)
	{
		COUT_LOG(LOG_CERROR, "Network link closed");
		return;
	}

	LoginSysMsgCmd uIdentification = (LoginSysMsgCmd)playerInfo->m_pMsg->netMessageHead.uIdentification;
	Cis is((char*)playerInfo->m_pData);

	switch (uIdentification)
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

	DPlayerPrepClient->CreatePlayer(playerInfo->m_pMsg->uIndex, playerInfo->m_pTcpSockInfo, id, pw);

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
	DPlayerPrepClient->LoadOneSql(id, "useraccount", data);

	if (data.empty())
	{
		userId = DUtil.CreateUserId();

		Cos os;
		os << passwaed << userId;

		DPlayerPrepClient->SaveReplaceSQL("useraccount", id, os);

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

