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
	PlayerPrepClient*	m_PlayerPrepClient;
};