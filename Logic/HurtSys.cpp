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
	int sceneid = 0, skillid = 0, norattack = 0;
	uint64_t hitedid = 0, behitedid = 0;

	is >> sceneid >> hitedid >> behitedid >> skillid >> norattack;

	Animal* hited = DSC->GetSceneAnimal(sceneid, hitedid);
	Animal* behited = DSC->GetSceneAnimal(sceneid, behitedid);
	
	if (norattack == 0)			// �չ�
	{
		NormalAttack(hited, behited);
	}
	else if (norattack == 1)	// ����
	{
		SkillAttack(hited, behited, skillid);
	}

	return true;
}

// ��ͨ����
void HurtSys::NormalAttack(Animal* hited, Animal* behited)
{
	if (!hited || !behited)
	{
		COUT_LOG(LOG_CERROR, "hited = null behited = null");
		return;
	}

	hited->GetSkillSys().ProcessAttacks(SkillSysMsgCmd::ssmc_norattack, behited);
}

// ���ܹ���
void HurtSys::SkillAttack(Animal* hited, Animal* behited, int skillid)
{
	if (!hited)
	{
		COUT_LOG(LOG_CERROR, "hited = null behited = null");
		return;
	}

	SkillCfg& skillCfg = CfgMgr->GetSkillCfg();
	const CHeroList* pCHeroList = skillCfg.GetCHeroListCfg(hited->GetAnimalid());
	if (!pCHeroList)
	{
		COUT_LOG(LOG_CERROR, "pCHeroList = null");
		return;
	}
	if (skillid > pCHeroList->skillId.size() || skillid <= 0)
	{
		COUT_LOG(LOG_CERROR, "skillid > pCHeroList->skillId.size() || skillid <= 0");
		return;
	}
	int relSkillID = pCHeroList->skillId[(size_t)skillid - 1];
	const CSkillIdList* pCSkillIdList = skillCfg.GetCSkillIdListCfg(relSkillID);
	if (!pCSkillIdList)
	{
		COUT_LOG(LOG_CERROR, "pCSkillIdList = null");
		return;
	}
	hited->GetSkillSys().ProcessAttacks(SkillSysMsgCmd::ssmc_norattack, behited, pCSkillIdList);
}