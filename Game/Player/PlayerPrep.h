#pragma once

// ���Ԥ����
class PlayerPrep
{
public:
	PlayerPrep();
	virtual ~PlayerPrep();

protected:
	PlayerPrep(const PlayerPrep& my);
	PlayerPrep& operator=(const PlayerPrep& my);

public:
	// ��ʼ��
	void Init();
	// ��Ϣ�ɷ�
	void MessageDispatch(PlayerInfo* playerInfo);
	// ��ȡ��ʱ��
	CServerTimer* GetCServerTimer();
	// ��ȡ��������
	ConditionVariable& GetConditionVariable();

public:
	// ���ö�ʱ��
	bool SetTimer(TimerCmd uTimerID, unsigned int uElapse, unsigned char timerType = SERVERTIMER_TYPE_PERISIST);
	// ֹͣ��ʱ��
	bool KillTimer(TimerCmd uTimerID);

public:
	// ��ʱ����Ϣ�ص�
	bool CallBackFun(TimerCmd cmd);
	// ɾ����ʱ��
	void DelTimerCallback(TimerCmd cmd);
	// ������Ϣ�ص�
	bool CallBackFun(MsgCmd cmd, PlayerInfo* playerInfo);
	// ���ȫ�ֵĶ�ʱ���ص�
	void AddTimerCallback(TimerCmd cmd, std::function<void()>&& fun);
	// ���ȫ�ֵ�����ص�
	void AddNetCallback(MsgCmd cmd, std::function<void(PlayerInfo*)>&& fun);
	// ������ɫ
	void CreatePlayer(LoginData& loginData);

private:
	// ��Ϣ�ɷ�
	void MessageDispatch(MsgCmd cmd, PlayerInfo* playerInfo);

private:
	// �����������ݿ���
	ConditionVariable m_cond;
	// ���ݿ�������
	ListString        m_sqlList;	
	// �ص�����
	MapNetFun		  m_NetCBFunMap;
	// ��ʱ��
	CServerTimer*	  m_pServerTimer;
	// �ص�����
	MapTimerFunc	  m_TimerFunMap;
};