#pragma once

// 玩家类
class Player
{
public:
	typedef std::map<MsgCmd, std::function<void(PlayerInfo*)>> PlayerCallBackFunMap;	// 消息回调函数
public:
	Player(unsigned int index, const TCPSocketInfo* pSockInfo, std::string& userId);
	~Player();

public:
	// 分发消息
	void DispatchMessage(MsgCmd cmd, PlayerInfo* pPlayerInfo);
	// 初始化回调函数
	void CallBackFunInit();
	void SetPlayerPreproces(PlayerPreproces* pp) { m_PlayerPreproces = pp; }

	int GetIndex() { return m_index; }
	void LoadMysql();						// 加载数据库
	void EnterGame();						// 进入游戏
	bool EnterScene();						// 进入场景
	std::string GetUserId() const;			// 获取玩家id
	const TCPSocketInfo* GetTCPSocketInfo();// 获取玩家TCP的网络信息

	// 消息回调
	void AddCallBackFun(MsgCmd cmd, std::function<void(PlayerInfo*)>&& fun);
	bool CallBackFun(MsgCmd cmd, PlayerInfo* pPlayerInfo);

private:
	bool Move(PlayerInfo* pPlayerInfo);		// 移动

private:
	unsigned int			m_index;			// 玩家索引
	PlayerAttrs				m_PlayerAttrs;		// 玩家属性
	std::string				m_userId;			// 玩家id
	const TCPSocketInfo*	m_pTcpSockInfo;		// 玩家TCP的网络信息
	PlayerPreproces*		m_PlayerPreproces;	// 玩家预处理句柄
	PlayerCallBackFunMap	m_CallBackFunMap;	// 回调函数
};