#pragma once

class PlayerCenter
{
public:
	PlayerCenter();
	virtual ~PlayerCenter();

protected:
	PlayerCenter(const PlayerCenter& my);
	PlayerCenter& operator=(const PlayerCenter& my);

public:
	// 初始化
	void Init();
	// 创建玩家
	void CreatePlayer(LoginData& loginData);

public:
	// 获取在线玩家
	void GetSocketSet(std::vector<unsigned int>& socketVec);
	// 获取条件变量
	ConditionVariable& GetConditionVariable();
	// 根据userid获取玩家
	PlayerClient* GetPlayerByUserid(uint64_t userId);
	// 根据index获取玩家
	PlayerClient* GetPlayerByIndex(unsigned int index);

private:
	// 玩家加载处理线程
	void HandlerPlayerThread();
	void HandleLoadPlayer(LoginData& loginData);
	void HandleLogicLoadPlayer(LoginData& loginData);
	void HandleCrossLoadPlayer(LoginData& loginData);
	bool SwapLoadPlayerList(ListLoginData& LloadPlayerList, ListLoginData& RloadPlayerList, bool& run);

private:
	PlayerClient* GetPlayerLogicByUserid(uint64_t& userId);
	PlayerClient* GetPlayerCrossByUserid(uint64_t& userId);
	void AddMapPlayerClient(uint64_t& userid, PlayerClient* player);
	void DelMapPlayerClient(uint64_t& userid);

private:
	ConditionVariable				m_cond;
	ListLoginData					m_LoadPlayerList;
	VectorPlayerClient				m_PlayerClientVec;
	MapPlayerClient					m_MapPlayerClient;
};