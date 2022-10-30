#pragma once

class PlayerCenterClient : public PlayerCenter
{
public:
	PlayerCenterClient(PlayerPrepClient* ppc);
	virtual~PlayerCenterClient();

public:
	// 注册玩家中心相关的数据库回调
	void RegisterGlobalCallBack();

protected:
	PlayerCenterClient(const PlayerCenterClient& my);
	PlayerCenterClient& operator=(const PlayerCenterClient& my);
};