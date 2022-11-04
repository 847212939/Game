#pragma once

enum class CrossSysMsgCmd
{
	cs_logic_to_cross_login		= 1,		// 本服发给跨服登录
	cs_client_to_cross_logout	= 2,		// 退出跨服
	cs_logic_to_cross_close		= 3,		// 对方关闭了socket
	cs_cross_to_logic_logout	= 4,		// 跨服返回登录结果
};

class CrossSys
{
public:
	CrossSys(PlayerPrepClient* ppc);
	~CrossSys();

private:
	void Network(PlayerInfo* playerInfo);

private:
	// 客户端请求登录跨服
	bool LogicToCrossLogin(Netmsg& msg, PlayerInfo* playerInfo);
	// 系统主动放松断开链接
	bool CloseCross(Netmsg& msg, PlayerInfo* playerInfo);
};