#pragma once
class MonsterClient;

enum class HurtSysMsgCmd
{
	cs_attack		= 1,  // ¹¥»÷
	sc_skillcd		= 2,  // ¼¼ÄÜCD
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
	void TimerCallback();
	void RegisterSkillTimer();
	void UnRegisterSkillTimer();
	void SendSkillCD(Animal* animal, int position);
	bool SkillCountdown(Animal* animal, int& cnt, int position);

private:
	bool CalHurt(Cis& is, PlayerInfo* playerInfo);

private:
	void NormalAttack(Animal* hited, Animal* behited);
	void SkillAttack(Animal* hited, Animal* behited, int skillpos);

private:
	SkillCDList m_SkillCDList;
};