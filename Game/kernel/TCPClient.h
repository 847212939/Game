#pragma once

class PlayerPrepClient;
class TCPClient : public TCPSocket
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
	bool Init(ServiceType serverType);
	// 通知所有线程进行一次循环
	void NotifyAll();

public:
	// 获取玩家预处理
	PlayerPrepClient* GetPlayerPrepClient();
	// 判断是否为跨服服务器
	bool IsCross();

private:
	// 主线程运行都是单线程
	// 定时器回调函数
	void TimerCallback(void* pDataLineHead);
	// 主消息回调函数
	void SocketCallback(void* pDataLineHead);
	// 断开链接回调函数
	void CloseSocketCallback(void* pDataLineHead);
	// 本服断开链接回调函数
	void CloseSocketLogicCallback(void* pDataLineHead); 
	// 跨服断开链接回调函数
	void CloseSocketCrossCallback(void* pDataLineHead);
	// 回调到TimerCallback or SocketCallback or CloseSocketCallback 中
	bool CallBackFun(SysMsgCmd cmd, void* pDataLineHead);
	// 添加回调函数
	void AddNetTypeCallback(SysMsgCmd cmd, std::function<void(void* pDataLineHead)>&& fun);

private:
	// 接收队列中消息的分发线程
	void HandlerRecvDataListThread();
	// 消息分发处理函数
	void HandleRecvData(ListItemData* pListItem);

private:
	// 获取服务器配置
	const CLogicCfg* GetServerCfg(ServiceType serverType);

private:
	MapTypeFunc			m_TypeFunMap;
	PlayerPrepClient*	m_PlayerPrepClient;
};