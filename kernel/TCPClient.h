#pragma once

class RegisteredAccount;
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
	RegisteredAccount* m_pRegisteredAccount;
};