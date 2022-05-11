#pragma once

class PlayerCenter
{
public:
	typedef std::map<SOCKET, Player*> SocketPlayerMap;
	typedef std::map<std::string, Player*> UserIdPlayerMap;

public:
	PlayerCenter(PlayerPreproces* pPlayerPreproces);
	virtual ~PlayerCenter();

public:
	// �ַ���Ϣ
	void DispatchMessage(MsgCmd cmd, PlayerInfo* pPlayerInfo);
	// ������ɫ
	bool CreatePlayr();

private:
	// ��ʼ���ص�����
	void InitCallBackFun();

private:
	// ���Ԥ����
	PlayerPreproces* m_pPlayerPreproces;
	// pair<SOCKET, Player*> һ��SOCKET��һ�����
	SocketPlayerMap m_SocketPlayerMap;
	// pair<userID, Player*> һ��userId��һ�����
	UserIdPlayerMap m_UserIdPlayerMap;
};