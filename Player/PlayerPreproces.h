#pragma once

// ���Ԥ����
class PlayerPreproces
{
public:
	// ����˻���Ϣ
	typedef std::map<std::string, std::string> AccountMap;
	// ���ݿ����list<sql>
	typedef std::list<std::string> SqlList;
	// ��Ϣ�ص�����
	typedef std::map<int, std::function<bool(PlayerInfo*)>> CallBackFunMap;

public:
	PlayerPreproces(TCPClient* pTCPClient);
	virtual ~PlayerPreproces();

public:
	// ������Ϣ
	void HandlerMessage(PlayerInfo* pInfo);
	// insert mysql
	void SaveInsertSQL(std::string sqlName, std::string name, std::string data, std::string keyName = "userid", std::string dataName = "data");
	// delete mysql
	void SaveDeleteSQL(std::string sqlName, const std::string& sCondition);
	// replace mysql
	void SaveReplaceSQL(std::string sqlName, std::string name, std::string data, std::string keyName= "userid", std::string dataName ="data");
	// update mysql
	void SaveUpdateSQL(std::string sqlName, std::string name, std::string data, const std::string& sCondition, std::string keyName = "userid", std::string dataName = "data");
	// ��ȡ֪ͨ��������
	ConditionVariable& GetConditionVariable();
	// ��ȡ����˻���Ϣ
	AccountMap& GetAccountMap();
	// ��ȡ������
	TCPClient* GetTCPClient();
	// ��ȡ���ݿ�
	CMysqlHelper& GetCMysqlHelper();
	// ��ȡ��ҹ���
	PlayerCenter& GetPlayerCenter();
	// ��ȡ�ص�����map
	CallBackFunMap& GetCallBackFunMap();

private:
	// ��ʼ��DB
	bool InitDB();
	// �߳�����
	bool Run();
	// ���ݿ�ִ��
	void HandlerExecuteDB();
	// �ַ���Ϣ
	void DispatchMessage();
	// ע���˺�
	bool RegisterAccount(std::string& id, std::string& passwaed);
	// ����˺���Ϣ
	bool CheckUserAccount(std::string& id, std::string& passwaed);
	// ��������˺���Ϣ
	std::string LoadUserAccount(std::string& id);
	// ������ɫ
	bool CreatePlayr();

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
	// �ص�����
	CallBackFunMap	m_CallBackFunMap;
	
};
