#pragma once

class Scene
{
public:
	Scene(PlayerPreproces* pPlayerPreproces);
	virtual ~Scene();

public:
	// 分发消息
	void DispatchMessage(MsgCmd cmd, PlayerInfo* pPlayerInfo);
	// 创建角色
	bool CreatePlayer(unsigned int index, const TCPSocketInfo* pSockInfo, std::string& userId);
	// 获取玩家预处理
	PlayerPreproces* GetPlayerPreproces() { return m_pPlayerPreproces; }
	// 获取玩家中心
	PlayerCenter& GetPlayerCenter() { return m_PlayerCenter; }

private:
	// 初始化回调函数
	void InitCallBackFun();

private:
	PlayerPreproces*		m_pPlayerPreproces;			// 玩家预处理
	PlayerCenter			m_PlayerCenter;				// 玩家中心
};
