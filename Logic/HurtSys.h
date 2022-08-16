#pragma once

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
	bool CalHurt(Animal* hited, Animal* behited);
	bool CalHurt(Cis& is, PlayerInfo* playerInfo);
};