#pragma once

class PlayerPrepClient;
class TCPClient : public CTCPSocketManage
{
public:
	TCPClient();
	virtual ~TCPClient();

protected:
	TCPClient(const TCPClient& my);
	TCPClient& operator=(const TCPClient& my);

public:
	bool Init(ServiceType serverType);
	void NotifyAll();

public:
	PlayerPrepClient* GetPlayerPrepClient();

private:
	void TimerCallback(void* pDataLineHead);
	void SocketCallback(void* pDataLineHead);
	void CloseSocketCallback(void* pDataLineHead);
	bool CallBackFun(SysMsgCmd cmd, void* pDataLineHead);
	void AddNetTypeCallback(SysMsgCmd cmd, std::function<void(void* pDataLineHead)>&& fun);

private:
	void HandlerRecvDataListThread();
	void HandleRecvData(ListItemData* pListItem);

private:
	TypeFunMap			m_TypeFunMap;
	PlayerPrepClient*	m_PlayerPrepClient;
};