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
	bool CreatePlayr(int index, const TCPSocketInfo* pSockInfo, std::string& userId);
	// ��ȡ���Ԥ����
	PlayerPreproces* GetPlayerPreproces();
	// ��ȡ�������
	PlayerCenter& GetPlayerCenter() { return m_PlayerCenter; }

private:
	// ��ʼ���ص�����
	void InitCallBackFun();

private:
	PlayerPreproces*		m_pPlayerPreproces;			// ���Ԥ����
	PlayerCenter			m_PlayerCenter;				// �������
};
