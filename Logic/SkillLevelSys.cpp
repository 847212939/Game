#include "../Game/stdafx.h"

SkillLevelSys::SkillLevelSys(PlayerClient* playerClient) : m_PlayerClient(playerClient)
{
	RegisterMysql(playerClient, this, SkillLevelSys::LoadMysql, "skilllevel");
	RegisterAttrs(playerClient, this, SkillLevelSys::CalAttrs);
	RegisterEnter(playerClient, this, SkillLevelSys::EnterScene);
	RegisterNetwk(playerClient, this, SkillLevelSys::NetWork, MsgCmd::MsgCmd_Move);
	RegisterLgout(playerClient, this, SkillLevelSys::Exit);
}

SkillLevelSys::~SkillLevelSys()
{
}

void SkillLevelSys::LoadMysql(std::string& data)
{

}

void SkillLevelSys::CalAttrs()
{

}

void SkillLevelSys::EnterScene()
{

}

void SkillLevelSys::NetWork(PlayerInfo* playerInfo)
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

	SkillLevelSysCmd uAssistantID = (SkillLevelSysCmd)playerInfo->pMsg->netMessageHead.uAssistantID;
	Cis is((char*)playerInfo->pData);

	switch (uAssistantID)
	{
	case SkillLevelSysCmd::cs_lv:
	{
		break;
	}
	default:
		break;
	}
}

void SkillLevelSys::Exit(SocketCloseLine* socketCloseLine)
{

}