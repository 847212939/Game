#pragma once

enum class CrossSysMsgCmd
{
	cs_logic_to_cross_login		= 1,		// �������������¼
	cs_logic_to_cross_logout	= 2,		// �˳����
};

class CrossSys
{
public:
	CrossSys(PlayerPrepClient* ppc);
	~CrossSys();

private:
	void Network(PlayerInfo* playerInfo);

private:
	bool CrossLogin(Netmsg& msg, PlayerInfo* playerInfo);
	bool CrossLogout(Netmsg& msg, PlayerInfo* playerInfo);

};