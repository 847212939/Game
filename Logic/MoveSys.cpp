#include "../Game/stdafx.h"

MoveSys::MoveSys(PlayerClient* playerClient) : m_PlayerClient(playerClient)
{
	RegisterLgout(m_PlayerClient, this, MoveSys::ExitCallback);
	RegisterAttrs(m_PlayerClient, this, MoveSys::EnterGameCallback);
	RegisterMysql(m_PlayerClient, this, MoveSys::MysqlCallback, "move");
	RegisterNetwk(m_PlayerClient, this, MoveSys::NetworkCallback, MsgCmd::MsgCmd_Move);
}

MoveSys::~MoveSys()
{
}

void MoveSys::MysqlCallback(std::string&& data)
{
	if (!m_PlayerClient)
	{
		COUT_LOG(LOG_CERROR, "MoveSys sub player is null");
		return;
	}
	if (data.empty())
	{
		return;
	}

	CIstringstream is(data);
	unsigned int x = 0, y = 0;

	is >> x >> y;

	COUT_LOG(LOG_CINFO, "x = %u, y = %u", x, y);
}

void MoveSys::EnterGameCallback()
{
	AttrsMap attrs;
	for (int i = 1; i < 10; i++)
	{
		attrs[i] += i;
	}

	AddAttributes(m_PlayerClient, attrs);
}

void MoveSys::NetworkCallback(PlayerInfo* pPlayerInfo)
{
	if (!m_PlayerClient)
	{
		COUT_LOG(LOG_CERROR, "MoveSys sub player is null");
		return;
	}
	if (!pPlayerInfo)
	{
		COUT_LOG(LOG_CERROR, "MoveSys Player Info is null");
		return;
	}
	if (!pPlayerInfo->m_pMsg || !pPlayerInfo->m_pTcpSockInfo)
	{
		COUT_LOG(LOG_CERROR, "MoveSys player info pMsg is null or player info sock info is null");
		return;
	}
	if (!pPlayerInfo->m_pTcpSockInfo->isConnect)
	{
		COUT_LOG(LOG_CERROR, "MoveSys Network link closed");
		return;
	}

	MoveSysMsgCmd uAssistantID = (MoveSysMsgCmd)pPlayerInfo->m_pMsg->netMessageHead.uAssistantID;
	CIstringstream is((char*)pPlayerInfo->m_pData);

	switch (uAssistantID)
	{
	case MoveSysMsgCmd::CS_Move:
	{
		MoveCoo(is, pPlayerInfo);
		break;
	}
	default:
		break;
	}

	return;
}

void MoveSys::ExitCallback(SocketCloseLine* pPlayerInfo)
{
	COUT_LOG(LOG_CINFO, "Íæ¼ÒÍË³ö");
}

bool MoveSys::MoveCoo(CIstringstream& is, PlayerInfo* pPlayerInfo)
{
	unsigned int x = 0, y = 0;
	is >> x >> y;

	COstringstream os;
	os << x << y;

	m_PlayerClient->SaveReplaceSQL("move", os);

	return true;
}
