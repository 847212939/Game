#include "../stdafx.h"

SkillCfg::SkillCfg()
{
}

SkillCfg::~SkillCfg()
{
}

void SkillCfg::ReadHeroList(CHeroList* config)
{
	m_HeroListSet.emplace(*config);
}

void SkillCfg::ReadSkillIdList(CSkillIdList* config)
{
	m_SkillIdListSet.emplace(*config);
}

const CHeroList* SkillCfg::GetCHeroListCfg(int heroid)
{
	SetTemplate<CHeroList>::const_iterator it = m_HeroListSet.find(heroid);
	if (it == m_HeroListSet.end())
	{
		return nullptr;
	}

	return &(*it);
}

const CSkillIdList* SkillCfg::GetCSkillIdListCfg(int skillid)
{
	SetTemplate<CSkillIdList>::const_iterator it = m_SkillIdListSet.find(skillid);
	if (it == m_SkillIdListSet.end())
	{
		return nullptr;
	}

	return &(*it);
}