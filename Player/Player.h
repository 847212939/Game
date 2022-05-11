#pragma once

// 玩家类
class Player
{
public:
	typedef long long UserId;

public:
	Player(TCPSocketInfo* sockInfo);
	~Player();

public:
	// 获取玩家id
	UserId GetUserId();
	// 获取玩家链接信息
	TCPSocketInfo* GetTCPSocketInfo();

private:
	// 玩家id
	UserId m_userId;
	// 玩家链接信息
	TCPSocketInfo* m_TCPSocketInfo;
};