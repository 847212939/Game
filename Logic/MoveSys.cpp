#include "../Game/stdafx.h"

MoveSys::MoveSys(PlayerClient* playerClient) : m_PlayerClient(playerClient)
{
	RegisterEnter(m_PlayerClient, this, MoveSys::EnterScene);
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

	Cis is(data);
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

void MoveSys::EnterScene()
{
	COUT_LOG(LOG_CINFO, "场景回调函数");
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
	if (!playerInfo->pMsg)
	{
		COUT_LOG(LOG_CERROR, "MoveSys player info pMsg is null or player info sock info is null");
		return;
	}

	MoveSysMsgCmd uAssistantID = (MoveSysMsgCmd)playerInfo->pMsg->netMessageHead.uAssistantID;
	Cis is((char*)playerInfo->pData);

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
	COUT_LOG(LOG_CINFO, "玩家退出");
}

bool MoveSys::MoveCoo(Cis& is, PlayerInfo* playerInfo)
{
	unsigned int x = 0, y = 0;
	is >> x >> y;

	Cos os;
	os << x << y;

	m_PlayerClient->SaveReplaceSQL("move", os);

	return true;
}
