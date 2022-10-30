#include "../stdafx.h"

SkillSys::SkillSys(PlayerPrepClient* ppc)
{
}

SkillSys::~SkillSys()
{

}

// ע�Ἴ��CD��ʱ��
void SkillSys::RegisterSkillTimer()
{
	if (!G_PlayerPrepClient->GetCServerTimer()[(int)(unsigned int)TimerCmd::TimerCmd_Skill % 
		G_BaseCfgMgr.GetTimerCnt()].ExistsTimer((unsigned int)TimerCmd::TimerCmd_Skill))
	{
		RegisterTimer(G_PlayerPrepClient, SkillSys::SkillCdTimer,
			TimerCmd::TimerCmd_Skill, 100, SERVERTIMER_TYPE_PERISIST);
	}
}

// ��ע�Ἴ��CD��ʱ��
void SkillSys::UnRegisterSkillTimer()
{
	if (G_PlayerPrepClient->GetCServerTimer()[(int)(unsigned int)TimerCmd::TimerCmd_Skill % 
		G_BaseCfgMgr.GetTimerCnt()].ExistsTimer((unsigned int)TimerCmd::TimerCmd_Skill))
	{
		UnRegisterTimer(G_PlayerPrepClient, TimerCmd::TimerCmd_Skill);
	}
}

void SkillSys::Network(PlayerInfo* playerInfo)
{
	if (!playerInfo)
	{
		Log(CERR, "Player Info is null");
		return;
	}
	HurtSysMsgCmd uAssistantID = (HurtSysMsgCmd)playerInfo->pMsg->netMessageHead.uAssistantID;
	Netmsg msg((char*)playerInfo->pData);

	switch (uAssistantID)
	{
	case HurtSysMsgCmd::cs_attack:
	{
		CalHurt(msg, playerInfo);
		break;
	}
	default:
		break;
	}
}

bool SkillSys::CalHurt(Netmsg& msg, PlayerInfo* playerInfo)
{
	int sceneid = 0, skillpos = 0, norattack = 0;
	uint64_t hitedid = 0, behitedid = 0;

	msg >> sceneid >> hitedid >> behitedid >> skillpos >> norattack;

	Animal* hited = G_SceneClient->GetSceneAnimal(sceneid, hitedid);
	Animal* behited = G_SceneClient->GetSceneAnimal(sceneid, behitedid);
	
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
	ListSkillCDData::iterator it = m_SkillCDList.begin();
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
		Netmsg msg;
		msg << value;
		dynamic_cast<PlayerClient*>(animal)->SendMsg(msg.str().c_str(), msg.str().size(), MsgCmd::MsgCmd_Hurt, (int)type, 0);

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
		Log(CERR, "animal = null or pCSkillIdList = null");
		return;
	}
	if (pCSkillIdList->skillCd > 0)
	{
		int skillpos = animal->GetSkillIdPos(pCSkillIdList);
		if (skillpos <= 0)
		{
			Log(CERR, "skillpos <= 0");
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
		Log(CERR, "animal = null or pCSkillIdList = null");
		return;
	}
	if (pCSkillIdList->skillCd > 0)
	{
		int skillpos = animal->GetSkillIdPos(pCSkillIdList);
		if (skillpos <= 0)
		{
			Log(CERR, "skillpos <= 0");
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
		Log(CERR, "animal = null or pCSkillIdList = null");
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
		Log(CERR, "hited = null or behited = null");
		return;
	}
	// ��ͨ����Ĭ�ϲ�λ��0
	if (!hited->GetSkillAcitve(0))
	{
		//Log(CINF, "���ܻ�����ȴ��");
		return;
	}
	const CSkillIdList* pCSkillIdList = hited->GetSkillIdListCfg(0);
	if (!pCSkillIdList)
	{
		Log(CERR, "pCSkillIdList = null");
		return;
	}

	AddNorSkillCD(hited, pCSkillIdList);
}

// ���ܹ���
void SkillSys::SkillAttack(Animal* hited, Animal* behited, int skillpos)
{
	if (!hited || !behited)
	{
		Log(CERR, "hited = null or behited = null");
		return;
	}
	if (!hited->GetSkillAcitve(skillpos))
	{
		//Log(CINF, "���ܻ�����ȴ��");
		return;
	}
	const CSkillIdList* pCSkillIdList = hited->GetSkillIdListCfg(skillpos);
	if (!pCSkillIdList)
	{
		Log(CERR, "pCSkillIdList = null");
		return;
	}

	// ����Ч���ͼ���cd
	AddSkillCDList(hited, behited, pCSkillIdList);
}
