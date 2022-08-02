#pragma once

class PlayerCenterClient : public PlayerCenter
{
public:
	PlayerCenterClient(PlayerPrepClient* ppc);
	virtual~PlayerCenterClient();
};