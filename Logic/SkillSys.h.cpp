#include "../Game/stdafx.h"

SkillSys::SkillSys(Animal* animal) : m_Animal(animal)
{
}

SkillSys::~SkillSys()
{
}

void SkillSys::ProcessAttacks(SkillSysMsgCmd cmd, Animal* behited, const CSkillIdList* pCSkillIdList)
{
	switch (cmd)
	{
	case SkillSysMsgCmd::ssmc_norattack:
	{
		NormalAttack(behited);
		break;
	}
	case SkillSysMsgCmd::ssmc_skillattack:
	{
		SkillAttack(behited, pCSkillIdList);
		break;
	}
	default:
		break;
	}
}

void SkillSys::NormalAttack(Animal* behited)
{

}

void SkillSys::SkillAttack(Animal* behited, const CSkillIdList* pCSkillIdList)
{

	// 技能效果和技能cd
	DSC->GetHurtSys().AddSkillCDList(m_Animal, behited, pCSkillIdList);
}