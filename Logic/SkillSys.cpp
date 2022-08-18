#include "../Game/stdafx.h"

SkillSys::SkillSys(PlayerPrepClient* ppc)
{
	RegisterNetwk(ppc, SkillSys::Network, MsgCmd::MsgCmd_Hurt);
}

SkillSys::~SkillSys()
{

}

// ע�Ἴ��CD��ʱ��
void SkillSys::RegisterSkillTimer()
{
	if (!DPPC->GetCServerTimer()[(int)(unsigned int)TimerCmd::TimerCmd_Skill % 
		BaseCfgMgr.GetTimerCnt()].
		ExistsTimer((unsigned int)TimerCmd::TimerCmd_Skill))
	{
		RegisterTimer(DPPC, SkillSys::SkillCdTimer,
			TimerCmd::TimerCmd_Skill, 100, SERVERTIMER_TYPE_PERISIST);
	}
}

// ��ע�Ἴ��CD��ʱ��
void SkillSys::UnRegisterSkillTimer()
{
	if (DPPC->GetCServerTimer()[(int)(unsigned int)TimerCmd::TimerCmd_Skill % 
		BaseCfgMgr.GetTimerCnt()].
		ExistsTimer((unsigned int)TimerCmd::TimerCmd_Skill))
	{
		UnRegisterTimer(DPPC, TimerCmd::TimerCmd_Skill);
	}
}

void SkillSys::Network(PlayerInfo* playerInfo)
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

bool SkillSys::CalHurt(Cis& is, PlayerInfo* playerInfo)
{
	int sceneid = 0, skillpos = 0, norattack = 0;
	uint64_t hitedid = 0, behitedid = 0;

	is >> sceneid >> hitedid >> behitedid >> skillpos >> norattack;

	Animal* hited = DSC->GetSceneAnimal(sceneid, hitedid);
	Animal* behited = DSC->GetSceneAnimal(sceneid, behitedid);
	
	if (norattack == 0)			// �չ�
	{
		NormalAttack(hited, behited, skillpos);
	}
	else if (norattack == 1)	// ����
	{
		SkillAttack(hited, behited, skillpos);
	}

	return true;
}

// ����ʱ�䶨ʱ��
void SkillSys::SkillCdTimer()
{
	SkillCDList::iterator it = m_SkillCDList.begin();
	while (it != m_SkillCDList.end())
	{
		SkillCDData& data = *it;
		if (!data.animal)
		{
			it = m_SkillCDList.erase(it);
			continue;
		}
		if (SkillCountdown(data.type, data.id, data.cnt, data.animal))
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
bool SkillSys::SkillCountdown(HurtSysMsgCmd type, int value, int& cnt, Animal* animal)
{
	--cnt;

	if (cnt > 0)
	{
		return false;
	}

	SendSkillCD(type, value, animal);
	return true;
}

// ����cdʱ�䵽֪ͨ�ͻ���
void SkillSys::SendSkillCD(HurtSysMsgCmd type, int value, Animal* animal)
{
	if (animal->GetType() == AnimalType::at_player)
	{
		Cos os;
		os << value;
		dynamic_cast<PlayerClient*>(animal)->SendData(os.str().c_str(), os.str().size(), MsgCmd::MsgCmd_Hurt, (int)type, 0);

		if (type == HurtSysMsgCmd::sc_skillcd)
		{
			animal->SetSkillAcitve(value, true);
		}
	}
	else
	{
		// С�ֵļ���CD �ȴ�����
	}
}

void SkillSys::AddSkillCDList(Animal* hited, Animal* behited, const CSkillIdList* pCSkillIdList)
{
	AddSkillCD(hited, pCSkillIdList);
	AddSkillEffectCD(behited, pCSkillIdList);
}

// ��ͨ����
void SkillSys::AddNorSkillCD(Animal* animal, const CSkillIdList* pCSkillIdList)
{
	if (!animal || !pCSkillIdList)
	{
		COUT_LOG(LOG_CERROR, "animal = null or pCSkillIdList = null");
		return;
	}
	if (pCSkillIdList->skillCd > 0)
	{
		int skillpos = animal->GetSkillIdPos(pCSkillIdList);
		if (skillpos <= 0)
		{
			COUT_LOG(LOG_CERROR, "skillpos <= 0");
			return;
		}
		int cnt = pCSkillIdList->skillCd - animal->GetAttrValue(AttrsCmd::attrs_gs);
		if (cnt <= 0)
		{
			cnt = pCSkillIdList->minSkillCd;
		}
		m_SkillCDList.push_back(SkillCDData(HurtSysMsgCmd::sc_skillcd, skillpos, cnt, animal));
		animal->SetSkillAcitve(skillpos, false);
		RegisterSkillTimer();
	}
}

// ����cd
void SkillSys::AddSkillCD(Animal* animal, const CSkillIdList* pCSkillIdList)
{
	if (!animal || !pCSkillIdList)
	{
		COUT_LOG(LOG_CERROR, "animal = null or pCSkillIdList = null");
		return;
	}
	if (pCSkillIdList->skillCd > 0)
	{
		int skillpos = animal->GetSkillIdPos(pCSkillIdList);
		if (skillpos <= 0)
		{
			COUT_LOG(LOG_CERROR, "skillpos <= 0");
			return;
		}
		int cnt = pCSkillIdList->skillCd - animal->GetAttrValue(AttrsCmd::attrs_scd);
		if (cnt <= 0)
		{
			cnt = pCSkillIdList->minSkillCd;
		}

		m_SkillCDList.push_back(SkillCDData(HurtSysMsgCmd::sc_skillcd, skillpos, cnt, animal));
		animal->SetSkillAcitve(skillpos, false);
		RegisterSkillTimer();
	}
}

// ����Ч��cd
void SkillSys::AddSkillEffectCD(Animal* animal, const CSkillIdList* pCSkillIdList)
{
	if (!animal || !pCSkillIdList)
	{
		COUT_LOG(LOG_CERROR, "animal = null or pCSkillIdList = null");
		return;
	}
	if (pCSkillIdList->skillEffectCd > 0)
	{
		int cnt = pCSkillIdList->skillEffectCd - animal->GetAttrValue(AttrsCmd::attrs_secd);
		if (cnt <= 0)
		{
			cnt = pCSkillIdList->minSkillEffect;
		}
		m_SkillCDList.push_back(SkillCDData(HurtSysMsgCmd::sc_effeckcd, pCSkillIdList->skillId, cnt, animal));
		RegisterSkillTimer();
	}
}

// ��ͨ����
void SkillSys::NormalAttack(Animal* hited, Animal* behited, int skillpos)
{
	if (!hited || !behited)
	{
		COUT_LOG(LOG_CERROR, "hited = null or behited = null");
		return;
	}
	// ��ͨ����Ĭ�ϲ�λ��0
	if (!hited->GetSkillAcitve(0))
	{
		//COUT_LOG(LOG_CINFO, "���ܻ�����ȴ��");
		return;
	}
	const CSkillIdList* pCSkillIdList = hited->GetSkillIdListCfg(0);
	if (!pCSkillIdList)
	{
		COUT_LOG(LOG_CERROR, "pCSkillIdList = null");
		return;
	}

	AddNorSkillCD(hited, pCSkillIdList);
}

// ���ܹ���
void SkillSys::SkillAttack(Animal* hited, Animal* behited, int skillpos)
{
	if (!hited || !behited)
	{
		COUT_LOG(LOG_CERROR, "hited = null or behited = null");
		return;
	}
	if (!hited->GetSkillAcitve(skillpos))
	{
		//COUT_LOG(LOG_CINFO, "���ܻ�����ȴ��");
		return;
	}
	const CSkillIdList* pCSkillIdList = hited->GetSkillIdListCfg(skillpos);
	if (!pCSkillIdList)
	{
		COUT_LOG(LOG_CERROR, "pCSkillIdList = null");
		return;
	}

	// ����Ч���ͼ���cd
	AddSkillCDList(hited, behited, pCSkillIdList);
}