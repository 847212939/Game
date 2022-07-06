#pragma once

// �����
class Player
{
public:

public:
	Player(const TCPSocketInfo* pSockInfo, std::string& userId);
	~Player();

public:
	void LoadMysql();						// �������ݿ�
	void EnterGame();						// ������Ϸ
	bool EnterScene();						// ���볡��
	std::string GetUserId() const;			// ��ȡ���id
	const TCPSocketInfo* GetTCPSocketInfo();// ��ȡ���TCP��������Ϣ

private:
	PlayerAttrs				m_PlayerAttrs;  // �������
	std::string				m_userId;		// ���id
	const TCPSocketInfo*	m_pTcpSockInfo;	// ���TCP��������Ϣ
};