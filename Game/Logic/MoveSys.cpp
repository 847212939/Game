#include "../stdafx.h"

MoveSys::MoveSys(PlayerClient* playerClient) : m_PlayerClient(playerClient)
{
	RegisterLgout(m_PlayerClient, MoveSys::Exit);
	RegisterAttrs(m_PlayerClient, MoveSys::CalAttrs);
	RegisterEnter(m_PlayerClient, MoveSys::EnterScene);
	RegisterNetwk(m_PlayerClient, MoveSys::NetWork, MsgCmd::MsgCmd_Move);
	RegisterPlayerMysql(m_PlayerClient, SLoadMysql(m_PlayerClient->GetIndex(), "move", MsgCmd::MsgCmd_Move, (unsigned int)MoveSysMsgCmd::cs_Load_mysql));
}

MoveSys::~MoveSys()
{
}

bool MoveSys::LoadMysql(Netmsg& msg, PlayerInfo* playerInfo)
{
	if (!m_PlayerClient)
	{
		Log(CERR, "MoveSys sub player is null");
		return false;
	}

	unsigned int x = 0, y = 0;
	msg >> x >> y;

	Log(CINF, "x = %u, y = %u", x, y);

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
		Log(CERR, "Player Info is null");
		return;
	}
	if (!playerInfo->pMsg)
	{
		Log(CERR, "player info pMsg is null or player info sock info is null");
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
	case MoveSysMsgCmd::cs_Load_mysql:
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
	Log(CINF, "Íæ¼ÒÍË³ö");
}

bool MoveSys::MoveCoo(Netmsg& msg, PlayerInfo* playerInfo)
{
	unsigned int x = 0, y = 0;
	msg >> x >> y;

	Netmsg msgos;
	msgos << x << y;

	SavePlayerMysql(m_PlayerClient->GetID(), "move", msgos);

	return true;
}
