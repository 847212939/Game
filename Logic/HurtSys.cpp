#include "../Game/stdafx.h"

HurtSys::HurtSys(PlayerPrepClient* ppc)
{
	RegisterNetwk(ppc, this, HurtSys::Network, MsgCmd::MsgCmd_Hurt);
}

HurtSys::~HurtSys()
{

}

void HurtSys::RegisterSkillTimer()
{
	RegisterTimer(DPPC, this, HurtSys::TimerCallback, TimerCmd::TimerCmd_Skill, 100, SERVERTIMER_TYPE_PERISIST);
}

void HurtSys::UnRegisterSkillTimer()
{
	UnRegisterTimer(DPPC, TimerCmd::TimerCmd_Skill);
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

// ����ʱ�䶨ʱ��
void HurtSys::TimerCallback()
{
	SkillCDList::iterator it = m_SkillCDList.begin();
	while (it != m_SkillCDList.end())
	{
		if (!it->second.second)
		{
			it = m_SkillCDList.erase(it);
			continue;
		}
		if (SkillCountdown(it->second.second, it->first, it->second.first))
		{
			it = m_SkillCDList.erase(it);
			continue;
		}

		++it;
	}
	if (m_SkillCDList.empty())
	{
		UnRegisterSkillTimer();
	}
}

// ����ʱ�䵹��ʱ
bool HurtSys::SkillCountdown(Animal* animal, int& cnt, int position)
{
	if (--cnt <= 0)
	{
		SendSkillCD(animal, position);
		return true;
	}

	return false;
}

void HurtSys::SendSkillCD(Animal* animal, int position)
{
	if (animal->GetType() == AnimalType::at_player)
	{
		Cos os;
		os << position;
		dynamic_cast<PlayerClient*>(animal)->SendData(os.str().c_str(), os.str().size(), MsgCmd::MsgCmd_Hurt, (int)HurtSysMsgCmd::sc_skillcd, 0);
	}
	else
	{
		// С�ֵļ���CD �ȴ�����
	}
}

// @cnt ����100ms
// @position ����λ��
void HurtSys::AddSkillCDList(Animal* animal, int cnt, int position)
{
	if (animal->GetType() == AnimalType::at_player)
	{
		m_SkillCDList.push_back({ cnt, { position, animal } });
	}
	else
	{

	}

	RegisterSkillTimer();
}