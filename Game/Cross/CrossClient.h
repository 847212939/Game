#pragma once

enum class CrossClientMsgCmd
{
	cs_logic_to_cross_login		= 1,		// 本服发给跨服登录
	cs_logic_to_cross_logout	= 2,		// 退出跨服
};

class CrossClient
{
public:
	CrossClient(PlayerClient* player);
	~CrossClient();

public:
	void Network(PlayerInfo* playerInfo);

public:
	// 退出跨服
	void LogoutCross();

private:
	bool LogicToCrossLogin(Netmsg& msg, PlayerInfo* playerInfo);

private:
	PlayerClient* m_Player;
};