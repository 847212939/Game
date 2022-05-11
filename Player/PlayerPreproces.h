#pragma once

// ���Ԥ����
class PlayerPreproces
{
	// ����˻���Ϣ
	typedef std::map<std::string, std::string> AccountMap;
	// ���ݿ����list<sql>
	typedef std::list<std::string> SqlList;
public:
	PlayerPreproces(TCPClient* pTCPClient);
	virtual ~PlayerPreproces();

public:
	// ������Ϣ
	void HandlerMessage(PlayerInfo* pInfo);
	// insert mysql
	bool SaveInsertSQL(std::string sqlName, std::string name, std::string data, std::string keyName = "userid", std::string dataName = "data");
	// delete mysql
	bool SaveDeleteSQL(std::string sqlName, const std::string& sCondition);
	// replace mysql
	bool SaveReplaceSQL(std::string sqlName, std::string name, std::string data, std::string keyName= "userid", std::string dataName ="data");
	// update mysql
	bool SaveUpdateSQL(std::string sqlName, std::string name, std::string data, const std::string& sCondition, std::string keyName = "userid", std::string dataName = "data");

private:
	// ��ʼ��DB
	bool InitDB();
	// ע���˺�
	bool RegisterAccount(std::string& id, std::string& passwaed);
	// ����˺���Ϣ
	bool CheckUserAccount(std::string& id, std::string& passwaed);
	// ��������˺���Ϣ
	std::string LoadUserAccount(std::string& id);
	// �߳�����
	bool Run();
	// ���ݿ�ִ��
	void HandlerExecuteDB();
	// �ַ���Ϣ
	void DispatchMessage();
	// ������ɫ
	bool CreatePlayr();
	// ��ȡ֪ͨ��������
	ConditionVariable& GetConditionVariable();

private:
	// �����������ݿ���
	ConditionVariable m_cond;
	// ���ݿ�������
	SqlList			m_sqlList;
	// ����
	TCPClient*		m_pTCPClient;
	// ������Ĺ�����
	PlayerCenter	m_PlayerCenter;
	// ���ݿ�
	CMysqlHelper	m_CMysqlHelper;
	//����˻���Ϣ
	AccountMap		m_accountMap;
	
};
