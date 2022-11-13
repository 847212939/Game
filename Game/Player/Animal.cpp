#include "../stdafx.h"

Animal::Animal() :
	m_Sceneid(0),
	m_ID(0),
	m_Lived(false),
	m_ResuTime(0),
	m_RefreshTime(0),
	m_Animalid(0),
	m_Animaltype(HeroType(0)),
	m_Animalname(""),
	m_Silencetime(0),
	m_Striketime(0),
	m_Confinementtime(0)
{
}

Animal::~Animal()
{

}

void Animal::AdditionAttributes(MapAttrsCmdInt& attrs)
{
	for (MapAttrsCmdInt::iterator it = attrs.begin(); it != attrs.end(); ++it)
	{
		if ((AttrsCmd)it->first <= AttrsCmd::attrs_begin || (AttrsCmd)it->first >= AttrsCmd::attrs_end)
		{
			Log(CERR, "δ֪���� ����AttrsCmd.hͷ�ļ� ����Ϊid:%d", it->first);
			continue;
		}
		m_AttrsMap[(AttrsCmd)it->first] += it->second;
	}
}

void Animal::RefreshProp()
{
	Netmsg msg;
	msg << (int)m_AttrsMap.size();
	for (MapAttrsCmdInt::const_iterator it = m_AttrsMap.begin(); it != m_AttrsMap.end(); ++it)
	{
		msg << (int)it->first << (int)it->second;
	}

	if (GetType() == AnimalType::at_player)
	{
		dynamic_cast<PlayerClient*>(this)->SendMsg(msg.os_str().c_str(), msg.os_size(), MsgCmd::MsgCmd_RefreshProperties, 1);
	}
}

int Animal::GetAttrValue(AttrsCmd attrType)
{
	MapAttrsCmdInt::iterator it = m_AttrsMap.find(attrType);
	if (it == m_AttrsMap.end())
	{
		return 0;
	}

	return it->second;
}

void Animal::SetAttrValue(AttrsCmd attrType, int attr)
{
	MapAttrsCmdInt::iterator it = m_AttrsMap.find(attrType);
	if (it == m_AttrsMap.end())
	{
		m_AttrsMap.emplace(std::make_pair(attrType, attr));
	}
	else
	{
		it->second += attr;
	}
}

// ���ݼ���λ��ȷ����������
const CSkillIdList* Animal::GetSkillIdListCfg(int skillpos)
{
	SkillCfg& skillCfg = G_CfgMgr->GetSkillCfg();
	const CHeroList* pCHeroList = skillCfg.GetCHeroListCfg(m_Animalid);
	if (!pCHeroList)
	{
		Log(CERR, "pCHeroList = null");
		return nullptr;
	}
	if (skillpos > pCHeroList->skillId.size() || skillpos <= 0)
	{
		Log(CERR, "skillid > pCHeroList->skillId.size() || skillid <= 0");
		return nullptr;
	}

	int relSkillID = pCHeroList->skillId[(size_t)skillpos];

	return skillCfg.GetCSkillIdListCfg(relSkillID);
}

// ���ݼ������û�ȡ����λ��
int Animal::GetSkillIdPos(const CSkillIdList* pCSkillIdList)
{
	if (!pCSkillIdList)
	{
		Log(CERR, "pCSkillIdList = null");
		return 0;
	}
	int pos = 0;
	SkillCfg& skillCfg = G_CfgMgr->GetSkillCfg();
	const CHeroList* pCHeroList = skillCfg.GetCHeroListCfg(m_Animalid);
	if (!pCHeroList)
	{
		Log(CERR, "pCHeroList = null");
		return 0;
	}
	for (auto& skillid : pCHeroList->skillId)
	{
		++pos;
		if (skillid == pCSkillIdList->skillId)
		{
			return pos;
		}
	}
	return 0;
}

void Animal::SetSkillAcitve(int pos, bool active)
{
	MapSkillData::iterator it = m_SkillDataMap.find(pos);
	if (it == m_SkillDataMap.end())
	{
		SkillData data;
		m_SkillDataMap.emplace(std::make_pair(pos, data));
	}
	else
	{
		it->second.cd = active;
	}
}

bool Animal::GetSkillAcitve(int pos)
{
	MapSkillData::iterator it = m_SkillDataMap.find(pos);
	if (it == m_SkillDataMap.end())
	{
		return true;
	}

	return it->second.cd;
}