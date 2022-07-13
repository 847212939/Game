#pragma once

class SubScene;
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
	void SetSubScene(SubScene* pSubScene);
	void DispatchMessage(MsgCmd cmd, PlayerInfo* pPlayerInfo);
	void CreatePlayer(unsigned int index, const TCPSocketInfo* pSockInfo, std::string& id, std::string& pw);

public:
	const SubScene* getScene();
	SubPlayer* GetSubPlayer(unsigned int index);
	const OnLinePlayerSet* GetSocketSet();
	ConditionVariable& GetConditionVariable();

private:
	void HandlerPlayerThread();

private:
	SubScene*					m_pScene;
	ConditionVariable			m_cond;
	LoadPlayerList				m_LoadPlayerList;
	std::vector<SubPlayer*>		m_pPlayerVec;
};