#pragma once

class TCPClient : public CTCPSocketManage
{
public:
	TCPClient();
	virtual ~TCPClient();

protected:
	TCPClient(const TCPClient& my);
	TCPClient& operator=(const TCPClient& my);

public:
	bool Init(bool& run, pfCallBackEvent func);
	void NotifyAll();

private:
	void SocketCallback(void* pDataLineHead);
	bool CallBackFun(SysMsgCmd cmd, void* pDataLineHead);
	void AddNetTypeCallback(SysMsgCmd cmd, std::function<void(void* pDataLineHead)>&& fun);

private:
	void HandlerRecvDataListThread();
	void HandleRecvData(ListItemData* pListItem);

private:
	TypeFunMap			m_TypeFunMap;
	pfCallBackEvent		m_CallBackFunc;
};