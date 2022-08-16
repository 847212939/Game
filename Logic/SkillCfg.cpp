#include "../Game/stdafx.h"

SkillCfg::SkillCfg()
{
}

SkillCfg::~SkillCfg()
{
}

void SkillCfg::ReadHeroList(CHeroList* config)
{
	m_HeroListSet.insert(*config);
}

void SkillCfg::ReadSkillIdList(CSkillIdList* config)
{
	m_SkillIdListSet.insert(*config);
}

const CHeroList* SkillCfg::GetCHeroListCfg(int heroid)
{
	CfgSet<CHeroList>::const_iterator it = m_HeroListSet.find(heroid);
	if (it == m_HeroListSet.end())
	{
		return nullptr;
	}

	return &(*it);
}

const CSkillIdList* SkillCfg::GetCSkillIdListCfg(int skillid)
{
	CfgSet<CSkillIdList>::const_iterator it = m_SkillIdListSet.find(skillid);
	if (it == m_SkillIdListSet.end())
	{
		return nullptr;
	}

	return &(*it);
}