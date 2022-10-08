#pragma once
class MonsterClient;

enum class HurtSysMsgCmd
{
	cs_attack		= 1,  // 攻击
	sc_skillcd		= 2,  // 技能CD
	sc_effeckcd		= 3,  // 技能效果CD
};

class SkillSys
{
public:
	SkillSys(PlayerPrepClient* ppc);
	virtual~SkillSys();

public:
	void AddSkillCDList(Animal* hited, Animal* behited, const CSkillIdList* pCSkillIdList);

private:
	void Network(PlayerInfo* playerInfo);

private:
	void SkillCdTimer();
	void RegisterSkillTimer();
	void UnRegisterSkillTimer();

private:
	bool CalHurt(Netmsg& is, PlayerInfo* playerInfo);

private:
	void SkillAttack(Animal* hited, Animal* behited, int skillpos);
	void NormalAttack(Animal* hited, Animal* behited, int skillpos);

private:
	// cd时间
	void AddSkillCD(Animal* animal, const CSkillIdList* pCSkillIdList);
	void AddNorSkillCD(Animal* animal, const CSkillIdList* pCSkillIdList);
	void AddSkillEffectCD(Animal* animal, const CSkillIdList* pCSkillIdList);
	void SendSkillCD(HurtSysMsgCmd type, int value, Animal* animal);
	bool SkillCountdown(HurtSysMsgCmd type, int value, int& cnt, Animal* animal);

private:
	SkillCDList m_SkillCDList;
};