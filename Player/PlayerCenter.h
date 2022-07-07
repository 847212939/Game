#pragma once

class Scene;
class PlayerCenter
{
public:
	// ������ҵ�KEY
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
	// �ַ���Ϣ
	void DispatchMessage(MsgCmd cmd, PlayerInfo* pPlayerInfo);
	// ������ɫ
	bool CreatePlayer(unsigned int index, const TCPSocketInfo* pSockInfo, std::string& userId);
	// ��ȡ���
	Player* GetPlayer(unsigned int index);

private:
	// ��Ҵ��������ݿ�ļ���
	void HandlerPlayerThread();

private:
	// ��ҳ���
	Scene*					m_pScene;
	ConditionVariable		m_cond;						// �����������ݿ���
	LoadPlayerList			m_LoadPlayerList;			// �����ͼ������
	std::vector<Player*>	m_pPlayerVec;				// �������
};