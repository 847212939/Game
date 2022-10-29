#pragma once

class PlayerCenter
{
public:
	PlayerCenter();
	virtual ~PlayerCenter();

protected:
	PlayerCenter(const PlayerCenter& my);
	PlayerCenter& operator=(const PlayerCenter& my);

public:
	// ��ʼ��
	void Init();
	// �������
	void CreatePlayer(LoginData& loginData);

public:
	// ��ȡ�������
	void GetSocketSet(std::vector<unsigned int>& socketVec);
	// ��ȡ��������
	std::condition_variable& GetConditionVariable();
	// ����userid��ȡ���
	PlayerClient* GetPlayerByUserid(uint64_t userId);
	// ����index��ȡ���
	PlayerClient* GetPlayerByIndex(unsigned int index);
	// ��ȡ��Ҷ���
	VectorPlayerClient& GetVectorPlayerClient();
	// ��ӿ�����
	void AddMapPlayerClient(uint64_t& userid, PlayerClient* player);
	// ɾ��������
	void DelMapPlayerClient(uint64_t& userid);

private:
	// ��Ҽ��ش����߳�
	void HandlerPlayerThread();
	void HandleLoadPlayer(LoginData& loginData);
	void HandleLogicLoadPlayer(LoginData& loginData);
	void HandleCrossLoadPlayer(LoginData& loginData);

private:
	PlayerClient* GetPlayerLogicByUserid(uint64_t& userId);
	PlayerClient* GetPlayerCrossByUserid(uint64_t& userId);

private:
	std::mutex						m_mutex;
	std::condition_variable			m_cond;
	ListLoginData					m_LoadPlayerList;
	VectorPlayerClient				m_PlayerClientVec;
	MapPlayerClient					m_MapPlayerClient;
};