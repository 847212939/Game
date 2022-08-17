#pragma once

struct CHeroList
{
	int heroId;
	int heroType;
	std::string heroName;
	IntVector skillId;

	void AddskillId(int skillid)
	{
		skillId.push_back(skillid);
	}

	CHeroList() : heroId(0), heroType(0), heroName("") {}
	CHeroList(int heroid) : heroId(heroid), heroType(0), heroName("") {}
	~CHeroList(){}

	bool operator < (const CHeroList& other) const
	{
		if (heroId != other.heroId) { return heroId < other.heroId; }
		return false;
	}
};

struct CSkillIdList
{
	int skillId;
	int skillType;
	int skillCd;
	int skillEffect;
	AttrsMap attrs;

	void AddAttrs(int id, int value)
	{
		attrs.insert({(AttrsCmd)id, value});
	}

	CSkillIdList() : skillId(0), skillType(0), skillCd(0), skillEffect(0){}
	CSkillIdList(int skillid) : skillId(skillid), skillType(0), skillCd(0), skillEffect(0) {}
	~CSkillIdList(){}

	bool operator < (const CSkillIdList& other) const
	{
		if (skillId != other.skillId) { return skillId < other.skillId; }
		return false;
	}

	int GetSkillEffect(AttrsCmd cmd)
	{
		AttrsMap::iterator it = attrs.find(cmd);
		if (it != attrs.end())
		{
			return it->second;
		}

		return 0;
	}
};

class SkillCfg
{
public:
	SkillCfg();
	~SkillCfg();

public:
	const CHeroList* GetCHeroListCfg(int heroid);
	const CSkillIdList* GetCSkillIdListCfg(int skillid);

public:
	void ReadHeroList(CHeroList* config);
	void ReadSkillIdList(CSkillIdList* config);

private:
	CfgSet<CHeroList>		m_HeroListSet;
	CfgSet<CSkillIdList>	m_SkillIdListSet;
};
