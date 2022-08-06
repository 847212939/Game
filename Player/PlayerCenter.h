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
	void SetSubScene(SceneClient* pSubScene);
	void DispatchMessage(MsgCmd cmd, PlayerInfo* pPlayerInfo);
	void CreatePlayer(unsigned int index, const TCPSocketInfo* pSockInfo, std::string& id, std::string& pw);

public:
	const SceneClient* getScene();
	const OnLinePlayerSet* GetSocketSet();
	ConditionVariable& GetConditionVariable();
	PlayerClient* GetSubPlayer(unsigned int index);

private:
	void HandlerPlayerThread();

private:
	SceneClient*					m_pScene;
	LoadPlayerList					m_LoadPlayerList;
	ConditionVariable				m_cond;
	std::vector<PlayerClient*>		m_pPlayerVec;
};