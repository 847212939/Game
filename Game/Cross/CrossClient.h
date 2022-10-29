#pragma once

enum class CrossClientMsgCmd
{
	cs_logic_to_cross_login		= 1,		// �������������¼
	cs_logic_to_cross_logout	= 2,		// �˳����
};

class CrossClient
{
public:
	CrossClient(PlayerClient* player);
	~CrossClient();

public:
	void Network(PlayerInfo* playerInfo);

public:
	// �˳����
	void LogoutCross();

private:
	bool LogicToCrossLogin(Netmsg& msg, PlayerInfo* playerInfo);

private:
	PlayerClient* m_Player;
};