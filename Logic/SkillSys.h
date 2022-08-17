#pragma once
class Animal;

enum class SkillSysMsgCmd
{
	ssmc_norattack		= 1,		// ÆÕÍ¨¹¥»÷
	ssmc_skillattack	= 2,		// ¼¼ÄÜ¹¥»÷
};

class SkillSys
{
public:
	SkillSys(Animal* animal);
	~SkillSys();

public:
	void ProcessAttacks(SkillSysMsgCmd cmd, Animal* behited, const CSkillIdList* pCSkillIdList = nullptr);

private:
	void NormalAttack(Animal* behited);
	void SkillAttack(Animal* behited, const CSkillIdList* pCSkillIdList);

private:
	Animal* m_Animal;
};