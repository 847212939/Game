#include "../Game/stdafx.h"

HurtSys::HurtSys(PlayerPrepClient* ppc)
{
	RegisterNetwk(ppc, this, HurtSys::Network, MsgCmd::MsgCmd_Hurt);
}

HurtSys::~HurtSys()
{

}

// ע�Ἴ��CD��ʱ��
void HurtSys::RegisterSkillTimer()
{
	if (!DPPC->GetCServerTimer()[(int)(unsigned int)TimerCmd::TimerCmd_Skill % 
		BaseCfgMgr.GetTimerCnt()].
		ExistsTimer((unsigned int)TimerCmd::TimerCmd_Skill))
	{
		RegisterTimer(DPPC, this, HurtSys::TimerCallback, 
			TimerCmd::TimerCmd_Skill, 100, SERVERTIMER_TYPE_PERISIST);
	}
}

// ��ע�Ἴ��CD��ʱ��
void HurtSys::UnRegisterSkillTimer()
{
	if (DPPC->GetCServerTimer()[(int)(unsigned int)TimerCmd::TimerCmd_Skill % 
		BaseCfgMgr.GetTimerCnt()].
		ExistsTimer((unsigned int)TimerCmd::TimerCmd_Skill))
	{
		UnRegisterTimer(DPPC, TimerCmd::TimerCmd_Skill);
	}
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
	int sceneid = 0, skillpos = 0, norattack = 0;
	uint64_t hitedid = 0, behitedid = 0;

	is >> sceneid >> hitedid >> behitedid >> skillpos >> norattack;

	Animal* hited = DSC->GetSceneAnimal(sceneid, hitedid);
	Animal* behited = DSC->GetSceneAnimal(sceneid, behitedid);
	
	if (norattack == 0)			// �չ�
	{
		NormalAttack(hited, behited);
	}
	else if (norattack == 1)	// ����
	{
		SkillAttack(hited, behited, skillpos);
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
void HurtSys::SkillAttack(Animal* hited, Animal* behited, int skillpos)
{
	if (!hited)
	{
		COUT_LOG(LOG_CERROR, "hited = null behited = null");
		return;
	}
	const CSkillIdList* pCSkillIdList = hited->GetSkillIdListCfg(skillpos);
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

void HurtSys::AddSkillCDList(Animal* animal, const CSkillIdList* pCSkillIdList)
{
	if (!animal || !pCSkillIdList)
	{
		COUT_LOG(LOG_CERROR, "animal = null or pCSkillIdList = null");
		return;
	}
	int skillpos = animal->GetSkillIdPos(pCSkillIdList);
	if (skillpos <= 0)
	{
		COUT_LOG(LOG_CERROR, "skillpos <= 0");
		return;
	}

	// ��CDװ��
	int surpluscd = pCSkillIdList->skillCd - animal->GetAttrValue(AttrsCmd::attrs_scd);
	if (surpluscd > 0)
	{
		m_SkillCDList.push_back({ surpluscd, { skillpos, animal } });
		RegisterSkillTimer();
	}
}