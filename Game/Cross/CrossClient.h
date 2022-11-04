#pragma once

enum class CrossClientMsgCmd
{
	cs_logic_to_cross_login		= 1,		// 本服发给跨服登录
	cs_client_to_cross_logout	= 2,		// 退出跨服
	cs_logic_to_cross_close		= 3,		// 对方关闭了socket
	cs_cross_to_logic_logout	= 4,		// 跨服返回登录结果
};

class CrossClient
{
public:
	CrossClient(PlayerClient* player);
	~CrossClient();

public:
	void Network(PlayerInfo* playerInfo);

public:
	// 系统主动放松断开链接
	bool CloseCross();

private:
	// 客户端请求登录跨服
	bool LogicToCrossLogin(Netmsg& msg, PlayerInfo* playerInfo);
	// 客户端请求断开链接
	bool ClientToCrossLogout(Netmsg& msg, PlayerInfo* playerInfo);

private:
	PlayerClient* m_Player;
};