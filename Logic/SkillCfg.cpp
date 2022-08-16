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