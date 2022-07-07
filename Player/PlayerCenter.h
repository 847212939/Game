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
	void CreatePlayer(unsigned int index, const TCPSocketInfo* pSockInfo, std::string& userId);

public:
	const SubScene* getScene();
	SubPlayer* GetSubPlayer(unsigned int index);

private:
	void HandlerPlayerThread();

private:
	SubScene*					m_pScene;
	ConditionVariable			m_cond;
	LoadPlayerList				m_LoadPlayerList;
	std::vector<SubPlayer*>		m_pPlayerVec;
};