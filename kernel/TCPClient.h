#pragma once

class PlayerPrepClient;
class TCPClient : public CTCPSocketManage
{
public:
	TCPClient();
	virtual ~TCPClient();

public:
	bool InitTCPClient(ServiceType serverType);
	void NotifyAll();

public:
	PlayerPrepClient* GetSubPlayerPreproces();

private:
	void TimerCallback(void* pDataLineHead);
	void SocketCallback(void* pDataLineHead);
	void CloseSocketCallback(void* pDataLineHead);
	bool CallBackFun(unsigned int cmd, void* pDataLineHead);
	void AddNetTypeCallback(int cmd, std::function<void(void* pDataLineHead)>&& fun);

private:
	void HandlerRecvDataListThread();

private:
	TypeFunMap			m_TypeFunMap;
	PlayerPrepClient*	m_PlayerPrepClient;
};