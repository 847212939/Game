#pragma once

// ���Ԥ����
class PlayerPreproces
{
public:
	// ע���¼��Э��
	enum class PlayerPreprocesCmd
	{
		cs_register     = 1,  // ע��
		cs_login        = 2,  // ��¼
	};
public:
	typedef std::map<std::string, std::string> AccountMap;						// ����˻���Ϣ<�˻�������>
	typedef std::map<std::string, std::string> AccountUserIDMap;				// ����˻�<�˻�, userId>
	typedef std::list<std::string> SqlList;										// ���ݿ����list<sql>	
	typedef std::map<MsgCmd, std::function<void(PlayerInfo*)>> CallBackFunMap;	// ��Ϣ�ص�����
	typedef std::map<std::string, std::string> SqlKeyDataMap;					// ���ݿ��ѯ���

public:
	PlayerPreproces(TCPClient* pTCPClient);
	virtual ~PlayerPreproces();

public:
	// ������Ϣ
	void HandlerMessage(PlayerInfo* pPlayerInfo);
	// ����һ�����ݿ�
	bool LoadOneSql(std::string userId, std::string sqlName, CMysqlHelper::MysqlData& queryData, std::string dataStr = "data");
	// ���ض������ݿ�
	bool LoadMulitySql(std::string userId, std::string sqlName, CMysqlHelper::MysqlData& queryData, std::string dataStr = "data");
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
	// ��ȡ����
	Scene& GetScene();
	// ��ȡ�ص�����map
	CallBackFunMap& GetCallBackFunMap();
	// ����ص�����
	void AddCallBackFun(MsgCmd cmd, std::function<void(PlayerInfo*)>&& fun);
	// �ص�����
	bool CallBackFun(MsgCmd cmd, PlayerInfo* pPlayerInfo);

private:
	// ��ʼ����Ϣ�ص�����
	void initCallBackFun();
	// ��ʼ��DB
	bool InitDB();
	// �߳�����
	bool Run();
	// ���ݿ�ִ��
	void HandlerExecuteSqlThread();
	// �ַ���Ϣ
	void DispatchMessage(MsgCmd cmd, PlayerInfo* pPlayerInfo);
	// ��¼
	void LoginInAccount(PlayerInfo* pPlayerInfo);
	// ע���˺�
	bool Register(std::string& id, std::string& passwaed, PlayerInfo* pPlayerInfo);
	// ����˺��Ƿ����
	bool LoginIn(std::string& id, std::string& passwaed, PlayerInfo* pPlayerInfo);
	// ��������˺���Ϣ
	std::string LoadUserAccount(std::string& id);
	// �������userid
	std::string LoadUserId(std::string& id);
	// ������ɫ
	bool CreatePlayr(PlayerInfo* pPlayerInfo);

private:
	ConditionVariable m_cond;				// �����������ݿ���
	SqlList           m_sqlList;			// ���ݿ�������
	TCPClient*        m_pTCPClient;			// ����ͻ���
	Scene             m_scene;				// ��ҳ���
	CMysqlHelper      m_CMysqlHelper;		// ���ݿ�
	AccountMap        m_accountMap;			// ����˻���Ϣ
	CallBackFunMap    m_CallBackFunMap;		// �ص�����
	AccountUserIDMap  m_AccountUserIDMap;	// �˻���userId
};
