#include "../stdafx.h"

SkillLvSys::SkillLvSys(PlayerClient* playerClient) : m_PlayerClient(playerClient)
{
	RegisterLgout(playerClient, SkillLvSys::Exit);
	RegisterAttrs(playerClient, SkillLvSys::CalAttrs);
	RegisterEnter(playerClient, SkillLvSys::EnterScene);
	RegisterNetwk(playerClient, SkillLvSys::NetWork, MsgCmd::MsgCmd_Skill);
	RegisterPlayerMysql(m_PlayerClient, SLoadMysql("skill", MsgCmd::MsgCmd_Skill, (unsigned int)SkillLvSysCmd::cs_load));
}

SkillLvSys::~SkillLvSys()
{
}

void SkillLvSys::LoadMysql(Netmsg& msg, PlayerInfo* playerInfo)
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
		Log(CERR, "Player Info is null");
		return;
	}
	if (!playerInfo->pMsg)
	{
		Log(CERR, "player info pMsg is null or player info sock info is null");
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
	case SkillLvSysCmd::cs_load:
	{
		LoadMysql(msg, playerInfo);
		break;
	}
	default:
		break;
	}
}

void SkillLvSys::Exit(SocketCloseLine* socketCloseLine)
{

}