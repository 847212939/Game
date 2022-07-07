#pragma once

class Scene;
class PlayerCenter
{
public:
	typedef std::list<LoadPlayerKey> LoadPlayerList;
public:
	PlayerCenter(Scene* pScene);
	virtual ~PlayerCenter();

public:
	void DispatchMessage(MsgCmd cmd, PlayerInfo* pPlayerInfo);
	bool CreatePlayer(unsigned int index, const TCPSocketInfo* pSockInfo, std::string& userId);
	SubPlayer* GetSubPlayer(unsigned int index);

private:
	void HandlerPlayerThread();

private:
	Scene*						m_pScene;
	ConditionVariable			m_cond;
	LoadPlayerList				m_LoadPlayerList;
	std::vector<SubPlayer*>		m_pPlayerVec;
};