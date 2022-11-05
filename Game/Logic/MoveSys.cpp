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
		return false;
	}

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
		return;
	}
	if (!playerInfo->pMsg)
	{
		return;
	}

	MoveSysMsgCmd uAssistantID = (MoveSysMsgCmd)playerInfo->pMsg->netMessageHead.uAssistantID;
	Netmsg msg((char*)playerInfo->pData, playerInfo->pMsg->uHandleSize, 0);

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
}

bool MoveSys::MoveCoo(Netmsg& msg, PlayerInfo* playerInfo)
{
	return true;
}
