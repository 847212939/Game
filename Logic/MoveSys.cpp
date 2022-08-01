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

void MoveSys::MysqlCallback(std::string& data)
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

void MoveSys::NetworkCallback(PlayerInfo* playerInfo)
{
	if (!m_PlayerClient)
	{
		COUT_LOG(LOG_CERROR, "MoveSys sub player is null");
		return;
	}
	if (!playerInfo)
	{
		COUT_LOG(LOG_CERROR, "MoveSys Player Info is null");
		return;
	}
	if (!playerInfo->m_pMsg || !playerInfo->m_pTcpSockInfo)
	{
		COUT_LOG(LOG_CERROR, "MoveSys player info pMsg is null or player info sock info is null");
		return;
	}
	if (!playerInfo->m_pTcpSockInfo->isConnect)
	{
		COUT_LOG(LOG_CERROR, "MoveSys Network link closed");
		return;
	}

	MoveSysMsgCmd uAssistantID = (MoveSysMsgCmd)playerInfo->m_pMsg->netMessageHead.uAssistantID;
	CIstringstream is((char*)playerInfo->m_pData);

	switch (uAssistantID)
	{
	case MoveSysMsgCmd::cs_move:
	{
		MoveCoo(is, playerInfo);
		break;
	}
	default:
		break;
	}

	return;
}

void MoveSys::ExitCallback(SocketCloseLine* socketCloseLine)
{
	COUT_LOG(LOG_CINFO, "����˳�");
}

bool MoveSys::MoveCoo(CIstringstream& is, PlayerInfo* playerInfo)
{
	unsigned int x = 0, y = 0;
	is >> x >> y;

	COstringstream os;
	os << x << y;

	m_PlayerClient->SaveReplaceSQL("move", os);

	return true;
}
