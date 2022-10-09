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
	void Init();

public:
	void MessageDispatch(MsgCmd cmd, PlayerInfo* playerInfo);
	void CreatePlayer(LoginData& loginData);

public:
	void GetSocketSet(std::vector<unsigned int>& socketVec);

	ConditionVariable& GetConditionVariable();
	PlayerClient* GetPlayerClientByUserid(uint64_t userId);
	PlayerClient* GetPlayerClientByIndex(unsigned int index);

private:
	void HandlerPlayerThread();
	void HandleLoadPlayer(LoginData& loginData);
	bool SwapLoadPlayerList(ListLoginData& LloadPlayerList, ListLoginData& RloadPlayerList, bool& run);

private:
	ConditionVariable				m_cond;
	ListLoginData					m_LoadPlayerList;
	VectorPlayerClient				m_PlayerClientVec;
};