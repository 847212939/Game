#pragma once

class PlayerCenter
{
	typedef std::map<SOCKET, Player*> SocketPlayerMap;
	typedef std::map<long long, Player*> UserIdPlayerMap;
public:
	PlayerCenter();
	virtual ~PlayerCenter();

public:
	// �ַ���Ϣ
	void DispatchMessage();
	// ������ɫ
	bool CreatePlayr();

private:
	// pair<SOCKET, Player*> һ��SOCKET��һ�����
	SocketPlayerMap m_SocketPlayerMap;
	// pair<userID, Player*> һ��userId��һ�����
	UserIdPlayerMap m_UserIdPlayerMap;
};