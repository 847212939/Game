#pragma once

// �����
class Player
{
public:
	typedef std::map<MsgCmd, std::function<void(PlayerInfo*)>> PlayerCallBackFunMap;	// ��Ϣ�ص�����
public:
	Player(unsigned int index, const TCPSocketInfo* pSockInfo, std::string& userId);
	~Player();

public:
	// �ַ���Ϣ
	void DispatchMessage(MsgCmd cmd, PlayerInfo* pPlayerInfo);
	// ��ʼ���ص�����
	void CallBackFunInit();
	void SetPlayerPreproces(PlayerPreproces* pp) { m_PlayerPreproces = pp; }

	int GetIndex() { return m_index; }
	void LoadMysql();						// �������ݿ�
	void EnterGame();						// ������Ϸ
	bool EnterScene();						// ���볡��
	std::string GetUserId() const;			// ��ȡ���id
	const TCPSocketInfo* GetTCPSocketInfo();// ��ȡ���TCP��������Ϣ

	// ��Ϣ�ص�
	void AddCallBackFun(MsgCmd cmd, std::function<void(PlayerInfo*)>&& fun);
	bool CallBackFun(MsgCmd cmd, PlayerInfo* pPlayerInfo);

private:
	bool Move(PlayerInfo* pPlayerInfo);		// �ƶ�

private:
	unsigned int			m_index;			// �������
	PlayerAttrs				m_PlayerAttrs;		// �������
	std::string				m_userId;			// ���id
	const TCPSocketInfo*	m_pTcpSockInfo;		// ���TCP��������Ϣ
	PlayerPreproces*		m_PlayerPreproces;	// ���Ԥ������
	PlayerCallBackFunMap	m_CallBackFunMap;	// �ص�����
};