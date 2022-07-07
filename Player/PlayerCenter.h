#pragma once

class SubScene;
class PlayerCenter
{
public:
	typedef std::list<LoadPlayerKey> LoadPlayerList;
public:
	PlayerCenter(SubScene* pSubScene);
	virtual ~PlayerCenter();

public:
	void DispatchMessage(MsgCmd cmd, PlayerInfo* pPlayerInfo);
	bool CreatePlayer(unsigned int index, const TCPSocketInfo* pSockInfo, std::string& userId);

public:
	SubPlayer* GetSubPlayer(unsigned int index);
	const SubScene* getScene();

private:
	void HandlerPlayerThread();

private:
	SubScene*					m_pScene;
	ConditionVariable			m_cond;
	LoadPlayerList				m_LoadPlayerList;
	std::vector<SubPlayer*>		m_pPlayerVec;
};