#pragma once

class PlayerPrepClient;
class TCPClient : public CTCPSocketManage
{
public:
	TCPClient();
	virtual ~TCPClient();

protected:
	// ���ÿ������캯��
	TCPClient(const TCPClient& my);
	TCPClient& operator=(const TCPClient& my);

public:
	// ��ʼ��
	bool Init(ServiceType serverType);
	// ֪ͨ�����߳̽���һ��ѭ��
	void NotifyAll();
	// ��ʼ���ͻ���
	bool InitNetwork(ClientInfo clientInfo);
	bool InitCallBack(NetworkCallBackFunc netFunc, 
		TimerCallBackFunc timerFunc, CloseCallBackFunc closeFunc);

public:
	// ��ȡ���Ԥ����
	PlayerPrepClient* GetPlayerPrepClient();
	TimerCallBackFunc GetTimerCallBackFunc();

private:
	// ���߳����ж��ǵ��߳�
	// ��ʱ���ص�����
	void TimerCallback(void* pDataLineHead);
	// ����Ϣ�ص�����
	void SocketCallback(void* pDataLineHead);
	// �Ͽ����ӻص�����
	void CloseSocketCallback(void* pDataLineHead);
	// �ص���TimerCallback or SocketCallback or CloseSocketCallback ��
	bool CallBackFun(SysMsgCmd cmd, void* pDataLineHead);
	// ��ӻص�����
	void AddNetTypeCallback(SysMsgCmd cmd, std::function<void(void* pDataLineHead)>&& fun);


private:
	// ���ն�������Ϣ�ķַ�
	void HandlerRecvDataListThread();
	void HandleRecvData(ListItemData* pListItem);

private:
	MapTypeFunc			m_TypeFunMap;
	NetworkCallBackFunc	m_NetworkCallBackFunc;
	TimerCallBackFunc	m_TimerCallBackFunc;
	CloseCallBackFunc	m_CloseCallBackFunc;
	PlayerPrepClient*	m_PlayerPrepClient;
};