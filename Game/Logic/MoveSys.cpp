#include "../stdafx.h"

MoveSys::MoveSys(PlayerClient* playerClient) : m_PlayerClient(playerClient)
{
	RegisterLgout(m_PlayerClient, MoveSys::Exit);
	RegisterAttrs(m_PlayerClient, MoveSys::CalAttrs);
	RegisterEnter(m_PlayerClient, MoveSys::EnterScene);
	RegisterNetwk(m_PlayerClient, MoveSys::NetWork, MsgCmd::MsgCmd_Move);
	RegisterMysql(m_PlayerClient, SLoadMysql("move", MsgCmd::MsgCmd_Move, (unsigned int)MoveSysMsgCmd::cs_Load));
}

MoveSys::~MoveSys()
{
}

bool MoveSys::LoadMysql(Netmsg& msg, PlayerInfo* playerInfo)
{
	if (!m_PlayerClient)
	{
		COUT_LOG(LOG_CERROR, "MoveSys sub player is null");
		return false;
	}

	unsigned int x = 0, y = 0;
	msg >> x >> y;

	COUT_LOG(LOG_CINFO, "x = %u, y = %u", x, y);

	return true;
}

void MoveSys::CalAttrs()
{
	MapAttrsCmdInt attrs;

	AddAttributes(m_PlayerClient, attrs);
}

void MoveSys::EnterScene()
{

}

void MoveSys::NetWork(PlayerInfo* playerInfo)
{
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
	Netmsg msg((char*)playerInfo->pData);

	switch (uAssistantID)
	{
	case MoveSysMsgCmd::cs_move:
	{
		MoveCoo(msg, playerInfo);
		break;
	}
	case MoveSysMsgCmd::cs_Load:
	{
		LoadMysql(msg, playerInfo);
		break;
	}
	default:
		break;
	}

	return;
}

void MoveSys::Exit(SocketCloseLine* socketCloseLine)
{
	COUT_LOG(LOG_CINFO, "Íæ¼ÒÍË³ö");
}

bool MoveSys::MoveCoo(Netmsg& msg, PlayerInfo* playerInfo)
{
	unsigned int x = 0, y = 0;
	msg >> x >> y;

	Netmsg msgos;
	msgos << x << y;

	m_PlayerClient->SaveReplaceSQL("move", msgos);

	return true;
}
