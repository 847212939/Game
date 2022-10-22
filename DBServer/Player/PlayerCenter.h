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

public:
	// �ַ���Ϣ
	void MessageDispatch(MsgCmd cmd, PlayerInfo* playerInfo);
	// �������
	void CreatePlayer(LoginData& loginData);

public:
	// ��ȡ�������
	void GetSocketSet(std::vector<unsigned int>& socketVec);
	// ��ȡ��������
	ConditionVariable& GetConditionVariable();
	// ����userid��ȡ���
	PlayerClient* GetPlayerClientByUserid(uint64_t userId);
	// ����index��ȡ���
	PlayerClient* GetPlayerClientByIndex(unsigned int index);

private:
	// ��Ҽ��ش����߳�
	void HandlerPlayerThread();
	void HandleLoadPlayer(LoginData& loginData);
	bool SwapLoadPlayerList(ListLoginData& LloadPlayerList, ListLoginData& RloadPlayerList, bool& run);

private:
	ConditionVariable				m_cond;
	ListLoginData					m_LoadPlayerList;
	VectorPlayerClient				m_PlayerClientVec;
};