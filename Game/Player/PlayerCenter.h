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
	ConditionVariable& GetConditionVariable();
	// ����userid��ȡ���
	PlayerClient* GetPlayerByUserid(uint64_t userId);
	// ����index��ȡ���
	PlayerClient* GetPlayerByIndex(unsigned int index);

private:
	// ��Ҽ��ش����߳�
	void HandlerPlayerThread();
	void HandleLoadPlayer(LoginData& loginData);
	void HandleLogicLoadPlayer(LoginData& loginData);
	void HandleCrossLoadPlayer(LoginData& loginData);
	bool SwapLoadPlayerList(ListLoginData& LloadPlayerList, ListLoginData& RloadPlayerList, bool& run);

private:
	PlayerClient* GetPlayerLogicByUserid(uint64_t& userId);
	PlayerClient* GetPlayerCrossByUserid(uint64_t& userId);
	void AddMapPlayerClient(uint64_t& userid, PlayerClient* player);
	void DelMapPlayerClient(uint64_t& userid);

private:
	ConditionVariable				m_cond;
	ListLoginData					m_LoadPlayerList;
	VectorPlayerClient				m_PlayerClientVec;
	MapPlayerClient					m_MapPlayerClient;
};