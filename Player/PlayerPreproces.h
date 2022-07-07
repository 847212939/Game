#pragma once

// ���Ԥ����
class PlayerPreproces
{
public:
	typedef std::list<std::string> SqlList;										// ���ݿ����list<sql>	
	typedef std::map<MsgCmd, std::function<void(PlayerInfo*)>> CallBackFunMap;	// ��Ϣ�ص�����
	typedef std::map<std::string, std::string> SqlKeyDataMap;					// ���ݿ��ѯ���

public:
	PlayerPreproces(TCPClient* pTCPClient);
	virtual ~PlayerPreproces();

public:
	// ������Ϣ
	void HandlerMessage(PlayerInfo* pPlayerInfo);

public:
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
	
public:
	SubScene& GetScene();
	TCPClient* GetTCPClient();
	CMysqlHelper& GetCMysqlHelper();
	ConditionVariable& GetConditionVariable();

public:
	// ����ص�����
	void AddCallBackFun(MsgCmd cmd, std::function<void(PlayerInfo*)>&& fun);
	// �ص�����
	bool CallBackFun(MsgCmd cmd, PlayerInfo* pPlayerInfo);

public:
	// ������ɫ
	bool CreatePlayer(unsigned int index, const TCPSocketInfo* pSockInfo, std::string& userId);

private:
	// ��ʼ��DB
	bool InitDB();
	// �߳�����
	bool RunThread();

private:
	// ���ݿ�ִ��
	void HandlerExecuteSqlThread();
	// �ַ���Ϣ
	void DispatchMessage(MsgCmd cmd, PlayerInfo* pPlayerInfo);

private:
	ConditionVariable m_cond;				// �����������ݿ���
	SqlList           m_sqlList;			// ���ݿ�������
	TCPClient*        m_pTCPClient;			// ����ͻ���
	SubScene          m_SubScene;			// ��ҳ���
	CMysqlHelper      m_CMysqlHelper;		// ���ݿ�
	CallBackFunMap    m_CallBackFunMap;		// �ص�����
};
