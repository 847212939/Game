#pragma once

// ���Ԥ����
class PlayerPreproces
{
public:
	PlayerPreproces(TCPClient* pTCPClient);
	virtual ~PlayerPreproces();

public:
	void Init();
	void HandlerMessage(PlayerInfo* pPlayerInfo);

public:
	// ����һ�����ݿ�
	std::string LoadOneSql(std::string userId, std::string sqlName, std::string dataStr = "data");
	std::string LoadOneSql(std::string sqlName, uint64_t userId, std::string dataStr = "data");
	bool LoadMulitySql(std::string sqlName, uint64_t userId, CMysqlHelper::MysqlData& queryData, std::string dataStr = "data");
	void CreateTableS(std::string name);
	void CreateTableI(std::string name);
	void CreateTableSql(const char* sql);
	void SaveInsertSQL(std::string sqlName, uint64_t userId, std::string data, std::string keyName = "userid", std::string dataName = "data");
	void SaveDeleteSQL(std::string sqlName, const std::string& sCondition);
	void SaveReplaceSQL(std::string sqlName, uint64_t userId, std::string data, std::string keyName = "userid", std::string dataName = "data");
	void SaveReplaceSQL(std::string sqlName, std::string userId, std::string data, std::string keyName = "userid", std::string dataName = "data");
	void SaveUpdateSQL(std::string sqlName, uint64_t userId, std::string data, const std::string& sCondition, std::string keyName = "userid", std::string dataName = "data");
	
public:
	SubScene& GetSubScene();
	TCPClient* GetTCPClient();
	CServerTimer* GetCServerTimer();
	CMysqlHelper& GetCMysqlHelper();

public:
	// ��ʱ��
	bool SetTimer(TimerCmd uTimerID, UINT uElapse, BYTE timerType = SERVERTIMER_TYPE_PERISIST);
	bool KillTimer(TimerCmd uTimerID);

public:
	bool CallBackFun(TimerCmd cmd);
	void DelTimerCallback(TimerCmd cmd);
	bool CallBackFun(MsgCmd cmd, PlayerInfo* pPlayerInfo);
	void AddTimerCallback(TimerCmd cmd, std::function<void()>&& fun);
	void AddNetCallback(MsgCmd cmd, std::function<void(PlayerInfo*)>&& fun);

public:
	// ������ɫ
	void CreatePlayer(unsigned int index, const TCPSocketInfo* pSockInfo, std::string& id, std::string& pw);

private:
	bool InitDB();
	void DispatchMessage(MsgCmd cmd, PlayerInfo* pPlayerInfo);

public:
	static char createptable[CreateTableLen];
	static char createpptable[CreateTableLen];

private:
	TCPClient*        m_pTCPClient;			// ����ͻ���
	SubScene          m_SubScene;			// ��ҳ���
	CMysqlHelper      m_CMysqlHelper;		// ���ݿ�
	NetFunMap		  m_NetCBFunMap;		// �ص�����
	CServerTimer*	  m_pServerTimer;		// ��ʱ��
	TimerFunMap		  m_TimerFunMap;		// �ص�����
};
