#pragma once

// 玩家预处理
class PlayerPrep
{
public:
	PlayerPrep();
	virtual ~PlayerPrep();

protected:
	PlayerPrep(const PlayerPrep& my);
	PlayerPrep& operator=(const PlayerPrep& my);

public:
	void Init();
	void HandlerMessage(PlayerInfo* playerInfo);

public:
	// 加载一条数据库
	void LoadOneSql(std::string& userId, std::string sqlName, std::string& outStr, std::string dataStr = "data");
	void LoadOneSql(std::string sqlName, uint64_t userId, std::string& outStr, std::string dataStr = "data");
	bool LoadMulitySql(std::string sqlName, uint64_t userId, CMysqlHelper::MysqlData& queryData, std::string dataStr = "data");
	void CreateTableS(std::string name);
	void CreateTableI(std::string name);
	void CreateTableSql(const char* sql);
	void SaveInsertSQL(std::string sqlName, uint64_t userId, std::string& data, std::string keyName = "userid", std::string dataName = "data");
	void SaveDeleteSQL(std::string sqlName, const std::string& sCondition);
	void SaveReplaceSQL(std::string sqlName, uint64_t userId, std::string& data, std::string keyName = "userid", std::string dataName = "data");
	void SaveReplaceSQL(std::string sqlName, std::string& userId, std::string data, std::string keyName = "userid", std::string dataName = "data");
	void SaveUpdateSQL(std::string sqlName, uint64_t userId, std::string& data, const std::string& sCondition, std::string keyName = "userid", std::string dataName = "data");
	
public:
	CServerTimer* GetCServerTimer();
	CMysqlHelper& GetSaveCMysqlHelper();
	CMysqlHelper& GetLoadCMysqlHelper();
	ConditionVariable& GetConditionVariable();

public:
	// 定时器
	bool SetTimer(TimerCmd uTimerID, UINT uElapse, BYTE timerType = SERVERTIMER_TYPE_PERISIST);
	bool KillTimer(TimerCmd uTimerID);

public:
	bool CallBackFun(TimerCmd cmd);
	void DelTimerCallback(TimerCmd cmd);
	bool CallBackFun(MsgCmd cmd, PlayerInfo* playerInfo);
	void AddTimerCallback(TimerCmd cmd, std::function<void()>&& fun);
	void AddNetCallback(MsgCmd cmd, std::function<void(PlayerInfo*)>&& fun);

public:
	// 创建角色
	void CreatePlayer(unsigned int index, std::string& id, std::string& pw);

private:
	bool InitDB();
	void HandlerExecuteSqlThread();
	void MessageDispatch(MsgCmd cmd, PlayerInfo* playerInfo);
	void HandleEexcuteMysql(SqlList& sqlList, std::string& sql);
	bool SwapMysqlList(SqlList& LSqlList, SqlList& RSqlList, bool& run);

public:
	static char createptable[CREATE_TABLE_LEN];
	static char createpptable[CREATE_TABLE_LEN];

private:
	ConditionVariable m_cond;				// 条件变量数据库用
	SqlList           m_sqlList;			// 数据库语链表
	NetFunMap		  m_NetCBFunMap;		// 回调函数
	CServerTimer*	  m_pServerTimer;		// 定时器
	TimerFunMap		  m_TimerFunMap;		// 回调函数
	CMysqlHelper      m_CMysqlHelperLoad;	// 数据库加载专用
	CMysqlHelper      m_CMysqlHelperSave;	// 数据库保存专用
};
