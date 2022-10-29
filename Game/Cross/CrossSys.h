#pragma once

enum class CrossSysMsgCmd
{
	cs_logic_to_cross_login = 1,		// 本服发给跨服登录
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

};