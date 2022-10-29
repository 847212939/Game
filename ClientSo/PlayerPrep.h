#pragma once

// 玩家预处理
class PlayerPrep
{
public:
	PlayerPrep();
	virtual ~PlayerPrep();

protected:
	PlayerPrep(const PlayerPrep& my);
	PlayerPrep& operator=(const PlayerPrep& my);

public:
	// 初始化
	void Init();
	// 获取定时器
	CServerTimer* GetCServerTimer();

public:
	// 设置定时器
	bool SetTimer(TimerCmd uTimerID, unsigned int uElapse, unsigned char timerType = SERVERTIMER_TYPE_PERISIST);
	// 停止定时器
	bool KillTimer(TimerCmd uTimerID);

public:
	// 定时器消息回调
	bool CallBackFun(TimerCmd cmd);
	// 删除定时器
	void DelTimerCallback(TimerCmd cmd);
	// 网络消息回调
	bool CallBackFun(MsgCmd cmd, PlayerInfo* playerInfo);
	// 添加全局的定时器回调
	void AddTimerCallback(TimerCmd cmd);
	// 添加全局的网络回调
	void AddNetCallback(MsgCmd cmd, std::function<void(PlayerInfo*)>&& fun);

private:
	void MessageLogicDispatch(PlayerInfo* playerInfo);

private:
	// 回调函数
	MapNetFun		  m_NetCBFunMap;
	// 定时器
	CServerTimer*	  m_pServerTimer;
	// 回调函数
	MapTimerFunc	  m_TimerFunMap;
};