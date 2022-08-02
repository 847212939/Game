#pragma once

class PlayerCenter
{
public:
	PlayerCenter();
	virtual ~PlayerCenter();

public:
	void Init();

public:
	void MessageDispatch(MsgCmd cmd, PlayerInfo* playerInfo);
	void CreatePlayer(unsigned int index, std::string& id, std::string& pw);

public:
	void GetSocketSet(std::vector<UINT>& socketVec);

	ConditionVariable& GetConditionVariable();
	PlayerClient* GetPlayerClientByUserid(uint64_t userId);
	PlayerClient* GetPlayerClientByIndex(unsigned int index);

private:
	void HandlerPlayerThread();
	void HandleLoadPlayer(LoadPlayerKey& loadPKey);
	bool SwapLoadPlayerList(LoadPlayerList& LloadPlayerList, LoadPlayerList& RloadPlayerList, bool& run);

private:
	ConditionVariable				m_cond;
	LoadPlayerList					m_LoadPlayerList;
	PlayerClientVec					m_PlayerClientVec;
};