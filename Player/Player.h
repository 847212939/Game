#pragma once

// 玩家类
class Player
{
public:

public:
	Player(const TCPSocketInfo* pSockInfo, std::string& userId);
	~Player();

public:
	void LoadMysql();						// 加载数据库
	void EnterGame();						// 进入游戏
	bool EnterScene();						// 进入场景
	std::string GetUserId() const;			// 获取玩家id
	const TCPSocketInfo* GetTCPSocketInfo();// 获取玩家TCP的网络信息

private:
	PlayerAttrs				m_PlayerAttrs;  // 玩家属性
	std::string				m_userId;		// 玩家id
	const TCPSocketInfo*	m_pTcpSockInfo;	// 玩家TCP的网络信息
};