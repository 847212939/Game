#pragma once

class Scene
{
public:
	Scene(PlayerPreproces* pPlayerPreproces);
	virtual ~Scene();

public:
	// �ַ���Ϣ
	void DispatchMessage(MsgCmd cmd, PlayerInfo* pPlayerInfo);
	// ������ɫ
	bool CreatePlayer(unsigned int index, const TCPSocketInfo* pSockInfo, std::string& userId);
	// ��ȡ���Ԥ����
	PlayerPreproces* GetPlayerPreproces() { return m_pPlayerPreproces; }
	// ��ȡ�������
	PlayerCenter& GetPlayerCenter() { return m_PlayerCenter; }

private:
	// ��ʼ���ص�����
	void InitCallBackFun();

private:
	PlayerPreproces*		m_pPlayerPreproces;			// ���Ԥ����
	PlayerCenter			m_PlayerCenter;				// �������
};
