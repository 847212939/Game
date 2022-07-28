#pragma once

class SubPlayerPreproces;
class TCPClient : public CTCPSocketManage
{
public:
	TCPClient();
	virtual ~TCPClient();

public:
	bool InitTCPClient(ServiceType serverType);
	void NotifyAll();

public:
	SubPlayerPreproces* GetSubPlayerPreproces();

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
	SubPlayerPreproces* m_SubPlayerPreproces;
};