#pragma once

class PlayerCenterClient : public PlayerCenter
{
public:
	PlayerCenterClient(PlayerPrepClient* ppc);
	virtual~PlayerCenterClient();

public:
	// ע�����������ص����ݿ�ص�
	void RegisterGlobalCallBack();

protected:
	PlayerCenterClient(const PlayerCenterClient& my);
	PlayerCenterClient& operator=(const PlayerCenterClient& my);
};