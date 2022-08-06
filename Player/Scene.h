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
	void SetSubPlayerPreproces(SubPlayerPreproces* pSubPlayerPreproces);

public:
	PlayerCenterClient& GetPlayerCenter();
	SubPlayerPreproces* GetPlayerPreproces();

private:
	PlayerCenterClient		m_PlayerCenterClient;			// �������
	SubPlayerPreproces*		m_SubPlayerPreproces;		// ���Ԥ����
};
