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
	COUT_LOG(LOG_CINFO, "定时器回调函数");
	UnRegisterTimer(DPPC, TimerCmd::TimerCmd_Test);
}

void LoginSys::NetworkCallback(PlayerInfo* playerInfo)
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
		// 账号不存在
		COUT_LOG(LOG_CINFO, "id or passworld is empty");
		return false;
	}

	// 数据库查询
	std::string data;
	DPPC->LoadOneSql(id, "useraccount", data);

	if (data.empty())
	{
		userId = DUtil.CreateUserId();

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

