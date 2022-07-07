#pragma once

class Scene;
class PlayerCenter
{
public:
	// 创建玩家的KEY
	struct LoadPlayerKey
	{
		int index;
		const TCPSocketInfo* pSockInfo;
		std::string userId;

		LoadPlayerKey(int nIndex, const TCPSocketInfo* sockInfo, std::string& Id) : index(nIndex), pSockInfo(sockInfo), userId(Id) {}
		~LoadPlayerKey() {}
	};
public:
	typedef std::list<LoadPlayerKey> LoadPlayerList;
public:
	PlayerCenter(Scene* pScene);
	virtual ~PlayerCenter();

public:
	// 分发消息
	void DispatchMessage(MsgCmd cmd, PlayerInfo* pPlayerInfo);
	// 创建角色
	bool CreatePlayer(unsigned int index, const TCPSocketInfo* pSockInfo, std::string& userId);
	// 获取玩家
	Player* GetPlayer(unsigned int index);

private:
	// 玩家创建和数据库的加载
	void HandlerPlayerThread();

private:
	// 玩家场景
	Scene*					m_pScene;
	ConditionVariable		m_cond;						// 条件变量数据库用
	LoadPlayerList			m_LoadPlayerList;			// 创建和加载玩家
	std::vector<Player*>	m_pPlayerVec;				// 在线玩家
};