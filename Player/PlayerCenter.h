#pragma once

class SceneClient;
class PlayerCenter
{
public:
	typedef std::list<LoadPlayerKey> LoadPlayerList;
public:
	PlayerCenter();
	virtual ~PlayerCenter();

public:
	void Init();

public:
	void SetSceneClient(SceneClient* sceneClient);
	void DispatchMessage(MsgCmd cmd, PlayerInfo* playerInfo);
	void CreatePlayer(unsigned int index, const TCPSocketInfo* pSockInfo, std::string& id, std::string& pw);

public:
	void GetSocketSet(std::vector<UINT>& socketVec);
	const SceneClient* GetSceneClient();
	ConditionVariable& GetConditionVariable();
	PlayerClient* GetPlayerClient(unsigned int index);

private:
	void HandlerPlayerThread();
	void CloseSocketEvent(unsigned int index);

private:
	ConditionVariable				m_cond;
	SceneClient*					m_SceneClient;
	LoadPlayerList					m_LoadPlayerList;
	std::vector<PlayerClient*>		m_pPlayerVec;
};