#pragma once
class MonsterClient;

enum class HurtSysMsgCmd
{
	cs_attack = 1,  // ¹¥»÷
};

class HurtSys
{
public:
	HurtSys(PlayerPrepClient* ppc);
	virtual~HurtSys();

private:
	void Network(PlayerInfo* playerInfo);

private:
	bool CalHurt(Cis& is, PlayerInfo* playerInfo);

private:
	int MonsterAttackPeople(MonsterClient* hited, Animal* behited, int skillid);
	int PeopleAttackMonster(PlayerClient* hited, Animal* behited, int skillid);
};