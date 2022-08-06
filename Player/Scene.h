#pragma once

class Scene
{
public:
	Scene();
	virtual ~Scene();

public:
	void Init();

public:
	void DispatchMessage(MsgCmd cmd, PlayerInfo* pPlayerInfo);
	void SetSubPlayerPreproces(PlayerPrepClient* pSubPlayerPreproces);

public:
	PlayerCenterClient& GetPlayerCenter();
	PlayerPrepClient* GetPlayerPreproces();

private:
	PlayerPrepClient*		m_PlayerPrepClient;		// ���Ԥ����
	PlayerCenterClient		m_PlayerCenterClient;	// �������
};
