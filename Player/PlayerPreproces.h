#pragma once

// ���Ԥ����
class PlayerPreproces
{
	// ���ݿ����unordered_map<tableName, sql>
	typedef std::unordered_map<std::string, std::string> SqlMap;
public:
	PlayerPreproces(TCPClient* pTCPClient);
	virtual ~PlayerPreproces();

public:
	// ��ʼ��DB
	bool InitDB();
	// ��������˺���Ϣ
	bool LoadUserAccount();
	// �߳�����
	bool Run();
	// ���ݿ�ִ��
	void HandlerExecuteDB();
	// ������Ϣ
	void HandlerMessage(PlayerInfo* pInfo);
	// �ַ���Ϣ
	void DispatchMessage();
	// ������ɫ
	bool CreatePlayr();
	// ��ȡ֪ͨ��������
	ConditionVariable& GetConditionVariable();
private:
	// ���ݿ�������
	SqlMap			m_sqlMap;
	// ����
	TCPClient*		m_pTCPClient;
	// ������Ĺ�����
	PlayerCenter	m_PlayerCenter;
	// ���ݿ�
	CMysqlHelper	m_CMysqlHelper;
	// �����������ݿ���
	ConditionVariable m_cond;
	
};
