#include "../Game/stdafx.h"

SkillLvSys::SkillLvSys(PlayerClient* playerClient) : m_PlayerClient(playerClient)
{
	RegisterMysql(playerClient, SkillLvSys::LoadMysql, "skill");
	RegisterAttrs(playerClient, SkillLvSys::CalAttrs);
	RegisterEnter(playerClient, SkillLvSys::EnterScene);
	RegisterNetwk(playerClient, SkillLvSys::NetWork, MsgCmd::MsgCmd_Skill);
	RegisterLgout(playerClient, SkillLvSys::Exit);
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

	SkillLvSysCmd uAssistantID = (SkillLvSysCmd)playerInfo->pMsg->netMessageHead.uAssistantID;
	Cis is((char*)playerInfo->pData);

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