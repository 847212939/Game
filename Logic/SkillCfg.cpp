#include "../Game/stdafx.h"

SkillCfg::SkillCfg()
{
}

SkillCfg::~SkillCfg()
{
}

void SkillCfg::ReadHeroList(HeroList* config)
{
	m_HeroListSet.insert(*config);
}

void SkillCfg::ReadSkillIdList(SkillIdList* config)
{
	m_SkillIdListSet.insert(*config);
}