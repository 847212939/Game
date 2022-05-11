#pragma once

// �����Ϣ
struct PlayerInfo
{
	void* pData;
	SocketType				uSrverType;
	SocketReadLine* pMsg;
	const TCPSocketInfo* pTcpSockInfo;

	PlayerInfo();
	~PlayerInfo();
};

// �����
class Player
{
public:
	typedef long long UserId;

public:
	Player(TCPSocketInfo* sockInfo);
	~Player();

public:
	// ��ȡ���id
	UserId GetUserId();
	// ��ȡ���������Ϣ
	TCPSocketInfo* GetTCPSocketInfo();

private:
	// ���id
	UserId m_userId;
	// ���������Ϣ
	TCPSocketInfo* m_TCPSocketInfo;
};