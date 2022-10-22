#pragma once

// ���Ԥ����
class PlayerPrep
{
public:
	PlayerPrep();
	virtual ~PlayerPrep();

protected:
	PlayerPrep(const PlayerPrep& my);
	PlayerPrep& operator=(const PlayerPrep& my);

public:
	// ��ʼ��
	void Init();
	// ��ʼ�����ݿ�
	bool InitDB();
	// ��Ϣ�ɷ�
	void MessageDispatch(PlayerInfo* playerInfo);

public:
	// �������ݿ�
	void LoadOneSql(std::string& userId, std::string sqlName, std::string& outStr, std::string dataStr = "data");
	void LoadOneSql(std::string sqlName, int serverid, uint64_t userId, std::string& outStr, std::string dataStr = "data");
	bool LoadMulitySql(std::string sqlName, uint64_t userId, CMysqlHelper::MysqlData& queryData, std::string dataStr = "data");
	// �������ݿ�
	void SaveInsertSQL(std::string sqlName, uint64_t userId, std::string& data, std::string keyName = "userid", std::string dataName = "data");
	void SaveDeleteSQL(std::string sqlName, const std::string& sCondition);
	void SaveReplaceSQL(std::string sqlName, uint64_t userId, std::string& data, std::string keyName = "userid", std::string dataName = "data");
	void SaveReplaceSQL(std::string sqlName, std::string& userId, std::string data, std::string keyName = "userid", std::string dataName = "data");
	void SaveUpdateSQL(std::string sqlName, uint64_t userId, std::string& data, const std::string& sCondition, std::string keyName = "userid", std::string dataName = "data");
	// �������ݿ��
	void CreateTableS(std::string name, int cnt = 4096);
	void CreateTableI(std::string name, int cnt = 4096);
	void CreateTableSql(const char* sql);

public:
	// ���ݿ�����
	static char createptable[CREATE_TABLE_LEN];
	static char createpptable[CREATE_TABLE_LEN];
	static char createdatabase[CREATE_TABLE_LEN];

public:
	// ��ȡ�������ݿ�
	CMysqlHelper& GetSaveCMysqlHelper();
	// ��ȡ�������ݿ�
	CMysqlHelper& GetLoadCMysqlHelper();
	// ��ȡ��������
	ConditionVariable& GetConditionVariable();

public:
	// ������Ϣ�ص�
	bool CallBackFun(MsgCmd cmd, PlayerInfo* playerInfo);
	// ���ȫ�ֵ�����ص�
	void AddNetCallback(MsgCmd cmd, std::function<void(PlayerInfo*)>&& fun);

private:
	// ִ�����ݿ�����߳�
	void HandlerExecuteSqlThread();
	void HandleEexcuteMysql(std::string& sql);
	bool SwapMysqlList(ListString& LSqlList, ListString& RSqlList, bool& run);
	// ��Ϣ�ɷ�
	void MessageDispatch(MsgCmd cmd, PlayerInfo* playerInfo);

private:
	// �����������ݿ���
	ConditionVariable m_cond;
	// ���ݿ�������
	ListString        m_sqlList;	
	// �ص�����
	MapNetFun		  m_NetCBFunMap;
	// ���ݿ����ר��
	CMysqlHelper      m_CMysqlHelperLoad;
	// ���ݿⱣ��ר��
	CMysqlHelper      m_CMysqlHelperSave;	
};