#pragma once

class PlayerPreproces;
class TCPClient : public CTCPSocketManage
{
public:
	TCPClient();
	virtual ~TCPClient();

private:
	// �շ����ݵ�ѭ�������߳����
	void Run();
	// �������յ��ͻ��˵���Ϣ����
	void HandlerRecvDataList();

private:
	// ע����Ϣ���
	PlayerPreproces* m_pRegisteredAccount;
};