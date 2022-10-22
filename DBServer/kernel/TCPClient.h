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
	bool Init(ServiceType serverType);
	// 通知所有线程进行一次循环
	void NotifyAll();

public:
	// 获取玩家预处理
	PlayerPrepClient* GetPlayerPrepClient();

private:
	void SocketCallback(void* pDataLineHead);

private:
	// 接收队列中消息的分发
	void HandlerRecvDataListThread();
	void HandleRecvData(ListItemData* pListItem);

private:
	PlayerPrepClient*	m_PlayerPrepClient;
};