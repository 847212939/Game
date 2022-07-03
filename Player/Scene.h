#pragma once

class Scene
{
public:
	Scene(PlayerPreproces* pPlayerPreproces);
	~Scene();

public:
	// �ַ���Ϣ
	void DispatchMessage(MsgCmd cmd, PlayerInfo* pPlayerInfo);
	// ������ɫ
	bool CreatePlayr(PlayerInfo* pPlayerInfo);
	// ��ȡ���Ԥ����
	PlayerPreproces* GetPlayerPreproces();

private:
	// ��ʼ���ص�����
	void InitCallBackFun();

private:
	PlayerPreproces* m_pPlayerPreproces;	// ���Ԥ����
	PlayerCenter     m_PlayerCenter;		// �������
};
