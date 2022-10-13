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

public:
	// 分发消息
	void MessageDispatch(MsgCmd cmd, PlayerInfo* playerInfo);
	// 创建玩家
	void CreatePlayer(LoginData& loginData);

public:
	// 获取在线玩家
	void GetSocketSet(std::vector<unsigned int>& socketVec);
	// 获取条件变量
	ConditionVariable& GetConditionVariable();
	// 根据userid获取玩家
	PlayerClient* GetPlayerClientByUserid(uint64_t userId);
	// 根据index获取玩家
	PlayerClient* GetPlayerClientByIndex(unsigned int index);

private:
	// 玩家加载处理线程
	void HandlerPlayerThread();
	void HandleLoadPlayer(LoginData& loginData);
	bool SwapLoadPlayerList(ListLoginData& LloadPlayerList, ListLoginData& RloadPlayerList, bool& run);

private:
	ConditionVariable				m_cond;
	ListLoginData					m_LoadPlayerList;
	VectorPlayerClient				m_PlayerClientVec;
};