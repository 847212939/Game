#pragma once

enum class CrossClientMsgCmd
{
	cs_cross_login = 1,		// �����֤
};

class CrossClient
{
public:
	CrossClient(PlayerClient* player);
	~CrossClient();

private:
	void Network(PlayerInfo* playerInfo);
};