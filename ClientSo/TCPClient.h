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

private:
	void SocketCallback(void* pDataLineHead);

private:
	// ���ն�������Ϣ�ķַ�
	void HandlerRecvDataListThread();
	void HandleRecvData(ListItemData* pListItem);

private:
	NetworkCallBackFunc	m_NetworkCallBackFunc;
	TimerCallBackFunc	m_TimerCallBackFunc;
	CloseCallBackFunc	m_CloseCallBackFunc;
	PlayerPrepClient*	m_PlayerPrepClient;
};