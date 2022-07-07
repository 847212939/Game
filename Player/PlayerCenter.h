#pragma once

class Scene;
class PlayerCenter
{
public:
	struct LoadPlayerKey
	{
		int index;
		const TCPSocketInfo* pSockInfo;
		std::string userId;

		LoadPlayerKey(int nIndex, const TCPSocketInfo* sockInfo, std::string& Id) : index(nIndex), pSockInfo(sockInfo), userId(Id) {}
		~LoadPlayerKey() {}
	};
public:
	typedef std::list<LoadPlayerKey> LoadPlayerList;
public:
	PlayerCenter(Scene* pScene);
	virtual ~PlayerCenter();

public:
	void DispatchMessage(MsgCmd cmd, PlayerInfo* pPlayerInfo);
	bool CreatePlayer(unsigned int index, const TCPSocketInfo* pSockInfo, std::string& userId);
	Player* GetPlayer(unsigned int index);

private:
	void HandlerPlayerThread();

private:
	Scene*					m_pScene;
	ConditionVariable		m_cond;
	LoadPlayerList			m_LoadPlayerList;
	std::vector<Player*>	m_pPlayerVec;
};