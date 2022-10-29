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
	// ��ȡ��ʱ��
	CServerTimer* GetCServerTimer();

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
	void AddTimerCallback(TimerCmd cmd);
	// ���ȫ�ֵ�����ص�
	void AddNetCallback(MsgCmd cmd, std::function<void(PlayerInfo*)>&& fun);

private:
	void MessageLogicDispatch(PlayerInfo* playerInfo);

private:
	// �ص�����
	MapNetFun		  m_NetCBFunMap;
	// ��ʱ��
	CServerTimer*	  m_pServerTimer;
	// �ص�����
	MapTimerFunc	  m_TimerFunMap;
};