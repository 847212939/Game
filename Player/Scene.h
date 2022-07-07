#pragma once

class Scene
{
public:
	Scene(SubPlayerPreproces* pSubPlayerPreproces);
	virtual ~Scene();

public:
	void DispatchMessage(MsgCmd cmd, PlayerInfo* pPlayerInfo);

public:
	SubPlayerCenter& GetPlayerCenter();
	SubPlayerPreproces* GetPlayerPreproces();

private:
	SubPlayerPreproces*		m_SubPlayerPreproces;		// ���Ԥ����
	SubPlayerCenter			m_SubPlayerCenter;			// �������
};
