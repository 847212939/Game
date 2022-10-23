#pragma once

class PlayerPrepClient;
class TCPClient : public CTCPSocketManage
{
public:
	TCPClient();
	virtual ~TCPClient();

protected:
	// 禁用拷贝构造函数
	TCPClient(const TCPClient& my);
	TCPClient& operator=(const TCPClient& my);

public:
	// 初始化
	bool InitDBServer();
	bool Init(ServiceType serverType);
	// 通知所有线程进行一次循环
	void NotifyAll();

public:
	// 获取玩家预处理
	PlayerPrepClient* GetPlayerPrepClient();

private:
	// 主线程运行都是单线程
	// 定时器回调函数
	void TimerCallback(void* pDataLineHead);
	// 主消息回调函数
	void SocketCallback(void* pDataLineHead);
	// 断开链接回调函数
	void CloseSocketCallback(void* pDataLineHead);
	// 回调到TimerCallback or SocketCallback or CloseSocketCallback 中
	bool CallBackFun(SysMsgCmd cmd, void* pDataLineHead);
	// 添加回调函数
	void AddNetTypeCallback(SysMsgCmd cmd, std::function<void(void* pDataLineHead)>&& fun);

private:
	// 接收队列中消息的分发
	void HandlerRecvDataListThread();
	void HandleRecvData(ListItemData* pListItem);

private:
	MapTypeFunc			m_TypeFunMap;
	PlayerPrepClient*	m_PlayerPrepClient;
};