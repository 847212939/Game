#include "../Game/stdafx.h"

MoveSys::MoveSys(PlayerClient* playerClient) : m_PlayerClient(playerClient)
{
	RegisterLgout(m_PlayerClient, MoveSys::Exit);
	RegisterAttrs(m_PlayerClient, MoveSys::CalAttrs);
	RegisterEnter(m_PlayerClient, MoveSys::EnterScene);
	RegisterMysql(m_PlayerClient, MoveSys::LoadMysql, "move");
	RegisterNetwk(m_PlayerClient, MoveSys::NetWork, MsgCmd::MsgCmd_Move);
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

	Netmsg is(data);
	unsigned int x = 0, y = 0;

	is >> x >> y;

	COUT_LOG(LOG_CINFO, "x = %u, y = %u", x, y);
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
	Netmsg is((char*)playerInfo->pData);

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
	COUT_LOG(LOG_CINFO, "Íæ¼ÒÍË³ö");
}

bool MoveSys::MoveCoo(Netmsg& is, PlayerInfo* playerInfo)
{
	unsigned int x = 0, y = 0;
	is >> x >> y;

	Netmsg msg;
	msg << x << y;

	m_PlayerClient->SaveReplaceSQL("move", msg);

	return true;
}
