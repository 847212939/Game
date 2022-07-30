#pragma once

class SceneClient;
class PlayerCenter
{
public:
	PlayerCenter();
	virtual ~PlayerCenter();

public:
	void Init();

public:
	void SetSceneClient(SceneClient* sceneClient);
	void MessageDispatch(MsgCmd cmd, PlayerInfo* playerInfo);
	void CreatePlayer(unsigned int index, const TCPSocketInfo* pSockInfo, std::string& id, std::string& pw);

public:
	void GetSocketSet(std::vector<UINT>& socketVec);

	SceneClient* GetSceneClient();
	ConditionVariable& GetConditionVariable();
	PlayerClient* GetPlayerClientByUserid(uint64_t userId);
	PlayerClient* GetPlayerClientByIndex(unsigned int index);

private:
	void HandlerPlayerThread();

private:
	ConditionVariable				m_cond;
	SceneClient*					m_SceneClient;
	LoadPlayerList					m_LoadPlayerList;
	std::vector<PlayerClient*>		m_PlayerClientVec;
};