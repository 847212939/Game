#include "../stdafx.h"

SkillLvSys::SkillLvSys(PlayerClient* playerClient) : m_PlayerClient(playerClient)
{
	RegisterLgout(playerClient, SkillLvSys::Exit);
	RegisterAttrs(playerClient, SkillLvSys::CalAttrs);
	RegisterEnter(playerClient, SkillLvSys::EnterScene);
	RegisterMysql(playerClient, SkillLvSys::LoadMysql, "skill");
	RegisterNetwk(playerClient, SkillLvSys::NetWork, MsgCmd::MsgCmd_Skill);
}

SkillLvSys::~SkillLvSys()
{
}

void SkillLvSys::LoadMysql(std::string& data)
{

}

void SkillLvSys::CalAttrs()
{

}

void SkillLvSys::EnterScene()
{

}

void SkillLvSys::NetWork(PlayerInfo* playerInfo)
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

	SkillLvSysCmd uAssistantID = (SkillLvSysCmd)playerInfo->pMsg->netMessageHead.uAssistantID;
	Netmsg msg((char*)playerInfo->pData);

	switch (uAssistantID)
	{
	case SkillLvSysCmd::cs_lv:
	{
		break;
	}
	default:
		break;
	}
}

void SkillLvSys::Exit(SocketCloseLine* socketCloseLine)
{

}