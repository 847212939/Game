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
	SubPlayerCenter& GetPlayerCenter();
	SubPlayerPreproces* GetPlayerPreproces();

private:
	SubPlayerPreproces*		m_SubPlayerPreproces;		// ���Ԥ����
	SubPlayerCenter			m_SubPlayerCenter;			// �������
};
