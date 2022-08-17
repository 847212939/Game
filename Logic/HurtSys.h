#pragma once
class MonsterClient;

enum class HurtSysMsgCmd
{
	cs_attack		= 1,  // 攻击
	sc_skillcd		= 2,  // 技能CD
};

class HurtSys
{
public:
	HurtSys(PlayerPrepClient* ppc);
	virtual~HurtSys();

public:
	void AddSkillCDList(Animal* animal, const CSkillIdList* pCSkillIdList);

private:
	void Network(PlayerInfo* playerInfo);

private:
	void SkillCdTimer();
	void RegisterSkillTimer();
	void UnRegisterSkillTimer();

private:
	bool CalHurt(Cis& is, PlayerInfo* playerInfo);

private:
	void NormalAttack(Animal* hited, Animal* behited);
	void SkillAttack(Animal* hited, Animal* behited, int skillpos);

private:
	// cd时间
	void SkillCD(Animal* animal, const CSkillIdList* pCSkillIdList);
	void SkillEffectCD(Animal* animal, const CSkillIdList* pCSkillIdList);
	void SendSkillCD(Animal* animal, int position);
	bool SkillCountdown(Animal* animal, int& cnt, int position);

private:
	SkillCDList m_SkillCDList;
};