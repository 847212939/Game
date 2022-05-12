#pragma once

class Scene;
class PlayerCenter
{
public:
	typedef std::map<SOCKET, Player*> SocketPlayerMap;
	typedef std::map<std::string, Player*> UserIdPlayerMap;

public:
	PlayerCenter(Scene* pScene);
	virtual ~PlayerCenter();

public:
	// 分发消息
	void DispatchMessage(MsgCmd cmd, PlayerInfo* pPlayerInfo);
	// 创建角色
	bool CreatePlayr();

private:
	// 初始化回调函数
	void InitCallBackFun();

private:
	// 玩家场景
	Scene*          m_pScene;
	// pair<SOCKET, Player*> 一个SOCKET绑定一个玩家
	SocketPlayerMap m_SocketPlayerMap;
	// pair<userID, Player*> 一个userId绑定一个玩家
	UserIdPlayerMap m_UserIdPlayerMap;
};