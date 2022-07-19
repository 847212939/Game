#pragma once

class SubPlayerPreproces;
class TCPClient : public CTCPSocketManage
{
public:
	TCPClient();
	virtual ~TCPClient();

public:
	bool InitTCPClient();
	void NotifyAll();

public:
	SubPlayerPreproces* GetSubPlayerPreproces();

private:
	void MysqlCallback(void* pDataLineHead);
	void TimerCallback(void* pDataLineHead);
	void SocketCallback(void* pDataLineHead);
	bool CallBackFun(int cmd, void* pDataLineHead);
	void AddTypeCallback(int cmd, std::function<void(void* pDataLineHead)>&& fun);

private:
	void HandlerRecvDataListThread();

private:
	TypeFunMap			m_TypeFunMap;
	SubPlayerPreproces* m_SubPlayerPreproces;
};