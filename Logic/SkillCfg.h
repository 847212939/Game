#pragma once

struct HeroList
{
	int heroId;
	int heroType;
	std::string heroName;
	IntVector skillId;

	void AddskillId(int skillid)
	{
		skillId.push_back(skillid);
	}

	HeroList() : heroId(0), heroType(0), heroName("") {}
	~HeroList(){}

	bool operator < (const HeroList& other) const
	{
		if (heroId != other.heroId) { return heroId < other.heroId; }
		return false;
	}
};

struct SkillIdList
{
	int skillId;
	AttrsMap attrs;

	void AddAttrs(int id, int value)
	{
		attrs.insert({(AttrsCmd)id, value});
	}

	SkillIdList() : skillId(0) {}
	~SkillIdList(){}

	bool operator < (const SkillIdList& other) const
	{
		if (skillId != other.skillId) { return skillId < other.skillId; }
		return false;
	}
};

class SkillCfg
{
public:
	SkillCfg();
	~SkillCfg();

public:
	void ReadHeroList(HeroList* config);
	void ReadSkillIdList(SkillIdList* config);

private:
	CfgSet<HeroList>	m_HeroListSet;
	CfgSet<SkillIdList> m_SkillIdListSet;
};
