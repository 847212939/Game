#pragma once

enum class CrossClientMsgCmd
{
	cs_cross_login = 1,		// �����֤
};

class CrossClient
{
public:
	CrossClient(PlayerPrepClient* ppc);
	~CrossClient();

private:
	void Network(PlayerInfo* playerInfo);

};