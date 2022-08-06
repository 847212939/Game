#include "../Game/stdafx.h"

LoginSys::LoginSys(PlayerPrepClient* playerPrepClient) : 
	m_PlayerPrepClient(playerPrepClient)
{
	Register();
}

LoginSys::~LoginSys()
{
}

void LoginSys::Register()
{
	RegisterNetwk(m_PlayerPrepClient, this, LoginSys::NetworkCallback, MsgCmd::MsgCmd_Login);
	RegisterTimer(m_PlayerPrepClient, this, LoginSys::TimerCallback, TimerCmd::TimerCmd_Test, 1000, SERVERTIMER_TYPE_PERISIST);
}

void LoginSys::TimerCallback()
{
	COUT_LOG(LOG_CINFO, "定时器回调函数");
	UnRegisterTimer(m_PlayerPrepClient, TimerCmd::TimerCmd_Test);
}

void LoginSys::NetworkCallback(PlayerInfo* playerInfo)
{
	if (!m_PlayerPrepClient)
	{
		COUT_LOG(LOG_CERROR, "Player preproces is null");
		return;
	}
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
	CIstringstream is((char*)playerInfo->m_pData);

	switch (uIdentification)
	{
	case LoginSysMsgCmd::CS_Login:
	{
		LoginIn(is, playerInfo);
		break;
	}
	default:
		break;
	}
}

bool LoginSys::LoginIn(CIstringstream& is, PlayerInfo* playerInfo)
{
	std::string id, pw;
	is >> id >> pw;

	m_PlayerPrepClient->CreatePlayer(playerInfo->m_pMsg->uIndex, playerInfo->m_pTcpSockInfo, id, pw);

	return true;
}

bool LoginSys::LoginIn(std::string& id, std::string& passwaed, uint64_t& userId)
{
	if (id.empty() || passwaed.empty())
	{
		// 账号不存在
		COUT_LOG(LOG_CINFO, "id or passworld is empty");
		return false;
	}

	// 数据库查询
	std::string data = m_PlayerPrepClient->LoadOneSql(id, "useraccount");
	if (data.empty())
	{
		userId = Util::Instance()->CreateUserId();

		COstringstream os;
		os << passwaed << userId;

		m_PlayerPrepClient->SaveReplaceSQL("useraccount", id, os);

		return true;
	}
	else
	{
		std::string pw;
		CIstringstream is(data);
		is >> pw >> userId;
		
		if (pw != passwaed)
		{
			// 密码不正确
			COUT_LOG(LOG_CINFO, "passworld is error");
			return false;
		}
		else
		{
			if (userId == 0)
			{
				COUT_LOG(LOG_CERROR, "账户存在异常");
				return false;
			}
		}
	}

	return true;
}

