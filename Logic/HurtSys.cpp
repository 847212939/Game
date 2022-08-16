#include "../Game/stdafx.h"

HurtSys::HurtSys(PlayerPrepClient* ppc)
{
	RegisterNetwk(ppc, this, HurtSys::Network, MsgCmd::MsgCmd_Hurt);
}

HurtSys::~HurtSys()
{

}

void HurtSys::Network(PlayerInfo* playerInfo)
{
	if (!playerInfo)
	{
		COUT_LOG(LOG_CERROR, "Player Info is null");
		return;
	}
	HurtSysMsgCmd uAssistantID = (HurtSysMsgCmd)playerInfo->pMsg->netMessageHead.uAssistantID;
	Cis is((char*)playerInfo->pData);

	switch (uAssistantID)
	{
	case HurtSysMsgCmd::cs_attack:
	{
		CalHurt(is, playerInfo);
		break;
	}
	default:
		break;
	}
}

bool HurtSys::CalHurt(Cis& is, PlayerInfo* playerInfo)
{
	int sceneid = 0;
	uint64_t hitedid = 0, behitedid = 0;
	int skillid = 0;

	is >> sceneid >> hitedid >> behitedid >> skillid;

	Animal* hited = DSC->GetSceneAnimal(sceneid, hitedid);
	Animal* behited = DSC->GetSceneAnimal(sceneid, behitedid);
	if (!hited)
	{
		COUT_LOG(LOG_CERROR, "hited = null");
		return false;
	}
	if (hited->GetType() == AnimalType::at_player)
	{
		PeopleAttackMonster(dynamic_cast<PlayerClient*>(hited), behited, skillid);
	}
	else if (hited->GetType() == AnimalType::at_monster)
	{
		MonsterAttackPeople(dynamic_cast<MonsterClient*>(hited), behited, skillid);
	}

	return true;
}

int HurtSys::PeopleAttackMonster(PlayerClient* hited, Animal* behited, int skillid)
{
	SkillCfg& skillCfg = CfgMgr->GetSkillCfg();
	const CHeroList* pCHeroList = skillCfg.GetCHeroListCfg(hited->GetHeroid());
	if (!pCHeroList)
	{
		COUT_LOG(LOG_CERROR, "pCHeroList = null");
		return 0;
	}
	if (skillid > pCHeroList->skillId.size() || skillid <= 0)
	{
		COUT_LOG(LOG_CERROR, "skillid > pCHeroList->skillId.size() || skillid <= 0");
		return 0;
	}
	int relSkillID = pCHeroList->skillId[(size_t)skillid - 1];
	
	return 0;
}

int HurtSys::MonsterAttackPeople(MonsterClient* hited, Animal* behited, int skillid)
{
	return 0;
}
