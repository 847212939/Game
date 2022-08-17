#include "../Game/stdafx.h"

SkillSys::SkillSys(PlayerPrepClient* ppc)
{
	RegisterNetwk(ppc, this, SkillSys::Network, MsgCmd::MsgCmd_Hurt);
}

SkillSys::~SkillSys()
{

}

// 注册技能CD定时器
void SkillSys::RegisterSkillTimer()
{
	if (!DPPC->GetCServerTimer()[(int)(unsigned int)TimerCmd::TimerCmd_Skill % 
		BaseCfgMgr.GetTimerCnt()].
		ExistsTimer((unsigned int)TimerCmd::TimerCmd_Skill))
	{
		RegisterTimer(DPPC, this, SkillSys::SkillCdTimer,
			TimerCmd::TimerCmd_Skill, 100, SERVERTIMER_TYPE_PERISIST);
	}
}

// 反注册技能CD定时器
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
	
	if (norattack == 0)			// 普攻
	{
		NormalAttack(hited, behited);
	}
	else if (norattack == 1)	// 技能
	{
		SkillAttack(hited, behited, skillpos);
	}

	return true;
}

// 技能时间定时器
void SkillSys::SkillCdTimer()
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

// 技能时间倒计时
bool SkillSys::SkillCountdown(Animal* animal, int& cnt, int position)
{
	--cnt;

	if (cnt > 0)
	{
		return false;
	}

	SendSkillCD(animal, position);
	return true;
}

void SkillSys::SendSkillCD(Animal* animal, int position)
{
	// 判断是否是技能id
	bool isSkillid = position > 100 ? true : false;
	if (animal->GetType() == AnimalType::at_player)
	{
		Cos os;
		os << isSkillid << position;
		dynamic_cast<PlayerClient*>(animal)->SendData(os.str().c_str(), os.str().size(), MsgCmd::MsgCmd_Hurt, (int)HurtSysMsgCmd::sc_skillcd, 0);
	}
	else
	{
		// 小怪的技能CD 等待处理
	}
}

void SkillSys::AddSkillCDList(Animal* hited, Animal* behited, const CSkillIdList* pCSkillIdList)
{
	SkillCD(hited, pCSkillIdList);
	SkillEffectCD(behited, pCSkillIdList);
}

// 减CD装备
void SkillSys::SkillCD(Animal* animal, const CSkillIdList* pCSkillIdList)
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
		int surpluscd = pCSkillIdList->skillCd - animal->GetAttrValue(AttrsCmd::attrs_scd);
		if (surpluscd > 0)
		{
			m_SkillCDList.push_back({ surpluscd, { skillpos, animal } });
			RegisterSkillTimer();
		}
		else
		{
			SendSkillCD(animal, skillpos);
		}
	}
}

// 技能效果cd
void SkillSys::SkillEffectCD(Animal* animal, const CSkillIdList* pCSkillIdList)
{
	if (!animal || !pCSkillIdList)
	{
		COUT_LOG(LOG_CERROR, "animal = null or pCSkillIdList = null");
		return;
	}
	if (pCSkillIdList->skillEffectCd > 0)
	{
		int effectcd = pCSkillIdList->skillEffectCd - animal->GetAttrValue(AttrsCmd::attrs_secd);
		if (effectcd > 0)
		{
			m_SkillCDList.push_back({ effectcd, { pCSkillIdList->skillId, animal } });
			RegisterSkillTimer();
		}
		else
		{
			SendSkillCD(animal, effectcd);
		}
	}
}

// 普通攻击
void SkillSys::NormalAttack(Animal* hited, Animal* behited)
{
	if (!hited || !behited)
	{
		COUT_LOG(LOG_CERROR, "hited = null behited = null");
		return;
	}

}

// 技能攻击
void SkillSys::SkillAttack(Animal* hited, Animal* behited, int skillpos)
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

	// 技能效果和技能cd
	AddSkillCDList(hited, behited, pCSkillIdList);
}
