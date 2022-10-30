#pragma once

enum class CrossClientMsgCmd
{
	cs_cross_login = 1,		// ¿ç·þÈÏÖ¤
};

class CrossClient
{
public:
	CrossClient(PlayerPrepClient* ppc);
	~CrossClient();

private:
	void Network(PlayerInfo* playerInfo);

};