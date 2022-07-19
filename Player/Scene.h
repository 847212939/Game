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
	SubPlayerCenter			m_SubPlayerCenter;			// �������
	SubPlayerPreproces*		m_SubPlayerPreproces;		// ���Ԥ����
};
