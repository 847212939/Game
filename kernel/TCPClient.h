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
	void Run();
	void HandlerRecvDataListThread();

private:
	SubPlayerPreproces* m_SubPlayerPreproces;
};