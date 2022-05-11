#pragma once

class PlayerCenter
{
	typedef std::map<SOCKET, Player*> SocketPlayerMap;
	typedef std::map<long long, Player*> UserIdPlayerMap;
public:
	PlayerCenter();
	virtual ~PlayerCenter();

public:
	// 分发消息
	void DispatchMessage();
	// 创建角色
	bool CreatePlayr();

private:
	// pair<SOCKET, Player*> 一个SOCKET绑定一个玩家
	SocketPlayerMap m_SocketPlayerMap;
	// pair<userID, Player*> 一个userId绑定一个玩家
	UserIdPlayerMap m_UserIdPlayerMap;
};