#pragma once
class MonsterClient;

enum class HurtSysMsgCmd
{
	cs_attack		= 1,  // ����
	sc_skillcd		= 2,  // ����CD
	sc_effeckcd		= 3,  // ����Ч��CD
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
	bool CalHurt(Cis& is, PlayerInfo* playerInfo);

private:
	void NormalAttack(Animal* hited, Animal* behited);
	void SkillAttack(Animal* hited, Animal* behited, int skillpos);

private:
	// cdʱ��
	void SkillCD(Animal* animal, const CSkillIdList* pCSkillIdList);
	void SkillEffectCD(Animal* animal, const CSkillIdList* pCSkillIdList);
	void SendSkillCD(Animal* animal, int position);
	bool SkillCountdown(Animal* animal, int& cnt, int position);

private:
	SkillCDList m_SkillCDList;
};