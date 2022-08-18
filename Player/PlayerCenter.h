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
	void GetSocketSet(std::vector<UINT>& socketVec);

	ConditionVariable& GetConditionVariable();
	PlayerClient* GetPlayerClientByUserid(uint64_t userId);
	PlayerClient* GetPlayerClientByIndex(unsigned int index);

private:
	void HandlerPlayerThread();
	void HandleLoadPlayer(LoginData& loginData);
	bool SwapLoadPlayerList(LoadPlayerList& LloadPlayerList, LoadPlayerList& RloadPlayerList, bool& run);

private:
	ConditionVariable				m_cond;
	LoadPlayerList					m_LoadPlayerList;
	PlayerClientVec					m_PlayerClientVec;
};