#include "../Game/stdafx.h"

MoveSys::MoveSys(PlayerClient* playerClient) : m_PlayerClient(playerClient)
{
	RegisterMysql(m_PlayerClient, MoveSys::LoadMysql, "move");
	RegisterAttrs(m_PlayerClient, MoveSys::CalAttrs);
	RegisterEnter(m_PlayerClient, MoveSys::EnterScene);
	RegisterNetwk(m_PlayerClient, MoveSys::NetWork, MsgCmd::MsgCmd_Move);
	RegisterLgout(m_PlayerClient, MoveSys::Exit);
}

MoveSys::~MoveSys()
{
}

void MoveSys::LoadMysql(std::string& data)
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

void MoveSys::CalAttrs()
{
	AttrsMap attrs;

	AddAttributes(m_PlayerClient, attrs);
}

void MoveSys::EnterScene()
{
	COUT_LOG(LOG_CINFO, "场景回调函数");
}

void MoveSys::NetWork(PlayerInfo* playerInfo)
{
	if (!m_PlayerClient)
	{
		COUT_LOG(LOG_CERROR, "sub player is null");
		return;
	}
	if (!playerInfo)
	{
		COUT_LOG(LOG_CERROR, "Player Info is null");
		return;
	}
	if (!playerInfo->pMsg)
	{
		COUT_LOG(LOG_CERROR, "player info pMsg is null or player info sock info is null");
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

void MoveSys::Exit(SocketCloseLine* socketCloseLine)
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
