#pragma once

class SubPlayerPreproces;
class TCPClient : public CTCPSocketManage
{
public:
	TCPClient();
	virtual ~TCPClient();

private:
	// �շ����ݵ�ѭ�������߳����
	void Run();
	// �������յ��ͻ��˵���Ϣ����
	void HandlerRecvDataListThread();

private:
	// ע����Ϣ���
	SubPlayerPreproces* m_SubPlayerPreproces;
};