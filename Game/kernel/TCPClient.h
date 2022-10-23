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
	bool InitDBServer();
	bool Init(ServiceType serverType);
	// ֪ͨ�����߳̽���һ��ѭ��
	void NotifyAll();

public:
	// ��ȡ���Ԥ����
	PlayerPrepClient* GetPlayerPrepClient();

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
	PlayerPrepClient*	m_PlayerPrepClient;
};