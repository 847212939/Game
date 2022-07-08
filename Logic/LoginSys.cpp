#include "../Game/stdafx.h"

LoginSys::LoginSys(SubPlayerPreproces* pSubPlayerPreproces) : 
	m_pSubPlayerPreproces(pSubPlayerPreproces)
{
	CallBackInit();
}

LoginSys::~LoginSys()
{
}

// 初始化消息回调函数
void LoginSys::CallBackInit()
{
	if (!m_pSubPlayerPreproces)
	{
		COUT_LOG(LOG_CERROR, "Player preproces is null");
		return;
	}
	m_pSubPlayerPreproces->AddNetCallback(MsgCmd::MsgCmd_Login, std::move(std::bind(&LoginSys::NetworkCallback, this, std::placeholders::_1)));
}

void LoginSys::NetworkCallback(PlayerInfo* pPlayerInfo)
{
	if (!m_pSubPlayerPreproces)
	{
		COUT_LOG(LOG_CERROR, "Player preproces is null");
		return;
	}
	if (!pPlayerInfo)
	{
		COUT_LOG(LOG_CERROR, "Player Info is null");
		return;
	}
	if (!pPlayerInfo->m_pMsg || !pPlayerInfo->m_pTcpSockInfo)
	{
		COUT_LOG(LOG_CERROR, "player info pMsg is null or player info sock info is null");
		return;
	}
	if (!pPlayerInfo->m_pTcpSockInfo->isConnect)
	{
		COUT_LOG(LOG_CERROR, "Network link closed");
		return;
	}

	LoginSysMsgCmd uAssistantID = (LoginSysMsgCmd)pPlayerInfo->m_pMsg->netMessageHead.uAssistantID;
	std::string str = (char*)pPlayerInfo->m_pData;
	CIstringstream is(str);

	switch (uAssistantID)
	{
	case LoginSysMsgCmd::CS_Login:
	{
		LoginIn(is, pPlayerInfo);
		break;
	}
	default:
		break;
	}
}

bool LoginSys::LoginIn(CIstringstream& is, PlayerInfo* pPlayerInfo)
{
	std::string id, pw;
	is >> id >> pw;

	if (!LoginIn(id, pw, pPlayerInfo))
	{
		return false;
	}
	m_pSubPlayerPreproces->CreatePlayer(pPlayerInfo->m_pMsg->uIndex, pPlayerInfo->m_pTcpSockInfo, pPlayerInfo->m_userId);

	return true;
}

bool LoginSys::LoginIn(std::string& id, std::string& passwaed, PlayerInfo* pPlayerInfo)
{
	if (id.empty() || passwaed.empty())
	{
		// 账号不存在
		COUT_LOG(LOG_CINFO, "id or passworld is empty");
		return false;
	}

	// 数据库查询
	std::string data = m_pSubPlayerPreproces->LoadOneSql(id, "useraccount");
	if (data.empty())
	{
		uint64_t userId = Util::Instance()->CreateUserId();

		COstringstream os;
		os << passwaed << userId;

		pPlayerInfo->m_userId = userId;
		m_pSubPlayerPreproces->SaveReplaceSQL("useraccount", id, os);

		return true;
	}
	else
	{
		std::string pw;
		uint64_t userId = 0;
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

			pPlayerInfo->m_userId = userId;
		}
	}

	return true;
}

