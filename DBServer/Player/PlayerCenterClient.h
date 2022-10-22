#pragma once

class PlayerCenterClient : public PlayerCenter
{
public:
	PlayerCenterClient(PlayerPrepClient* ppc);
	virtual~PlayerCenterClient();

protected:
	PlayerCenterClient(const PlayerCenterClient& my);
	PlayerCenterClient& operator=(const PlayerCenterClient& my);
};