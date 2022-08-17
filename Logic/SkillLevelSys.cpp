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

}

void SkillLevelSys::Exit(SocketCloseLine* socketCloseLine)
{

}