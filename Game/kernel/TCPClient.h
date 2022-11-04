#pragma once

class PlayerPrepClient;
class TCPClient : public TCPSocket
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
	// �ж��Ƿ�Ϊ���������
	bool IsCross();

private:
	// ���߳����ж��ǵ��߳�
	// ��ʱ���ص�����
	void TimerCallback(void* pDataLineHead);
	// ����Ϣ�ص�����
	void SocketCallback(void* pDataLineHead);
	// �Ͽ����ӻص�����
	void CloseSocketCallback(void* pDataLineHead);
	// �����Ͽ����ӻص�����
	void CloseSocketLogicCallback(void* pDataLineHead); 
	// ����Ͽ����ӻص�����
	void CloseSocketCrossCallback(void* pDataLineHead);
	// �ص���TimerCallback or SocketCallback or CloseSocketCallback ��
	bool CallBackFun(SysMsgCmd cmd, void* pDataLineHead);
	// ��ӻص�����
	void AddNetTypeCallback(SysMsgCmd cmd, std::function<void(void* pDataLineHead)>&& fun);

private:
	// ���ն�������Ϣ�ķַ��߳�
	void HandlerRecvDataListThread();
	// ��Ϣ�ַ�������
	void HandleRecvData(ListItemData* pListItem);

private:
	// ��ȡ����������
	const CLogicCfg* GetServerCfg(ServiceType serverType);

private:
	MapTypeFunc			m_TypeFunMap;
	PlayerPrepClient*	m_PlayerPrepClient;
};