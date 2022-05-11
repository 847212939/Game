#pragma once

class RegisteredAccount
{
	// ���ݿ����unordered_map<tableName, sql>
	typedef std::unordered_map<std::string, std::string> SqlMap;
public:
	RegisteredAccount(TCPClient* pTCPClient);
	virtual ~RegisteredAccount();

public:
	// �������ݿ�
	bool InitDB();
	//�߳�����
	bool Run();
	//	������Ϣ
	void HandelMessage(PlayerInfo* pInfo);
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
	TCPClient*		m_TCPClient;
	// ������Ĺ�����
	PlayerCenter	m_PlayerCenter;
	// ���ݿ�
	CMysqlHelper	m_CMysqlHelper;
	// �����������ݿ���
	ConditionVariable m_cond;
	
};
