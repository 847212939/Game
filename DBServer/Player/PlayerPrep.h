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
	// 初始化
	void Init();
	// 初始化数据库
	bool InitDB();
	// 消息派发
	void MessageDispatch(PlayerInfo* playerInfo);

public:
	// 加载数据库
	void LoadOneSql(std::string& userId, std::string sqlName, std::string& outStr, std::string dataStr = "data");
	void LoadOneSql(std::string sqlName, int serverid, uint64_t userId, std::string& outStr, std::string dataStr = "data");
	bool LoadMulitySql(std::string sqlName, uint64_t userId, CMysqlHelper::MysqlData& queryData, std::string dataStr = "data");
	// 保存数据库
	void SaveInsertSQL(std::string sqlName, uint64_t userId, std::string& data, std::string keyName = "userid", std::string dataName = "data");
	void SaveDeleteSQL(std::string sqlName, const std::string& sCondition);
	void SaveReplaceSQL(std::string sqlName, uint64_t userId, std::string& data, std::string keyName = "userid", std::string dataName = "data");
	void SaveReplaceSQL(std::string sqlName, std::string& userId, std::string data, std::string keyName = "userid", std::string dataName = "data");
	void SaveUpdateSQL(std::string sqlName, uint64_t userId, std::string& data, const std::string& sCondition, std::string keyName = "userid", std::string dataName = "data");
	// 创建数据库表
	void CreateTableS(std::string name, int cnt = 4096);
	void CreateTableI(std::string name, int cnt = 4096);
	void CreateTableSql(const char* sql);

public:
	// 数据库语句表
	static char createptable[CREATE_TABLE_LEN];
	static char createpptable[CREATE_TABLE_LEN];
	static char createdatabase[CREATE_TABLE_LEN];

public:
	// 获取保存数据库
	CMysqlHelper& GetSaveCMysqlHelper();
	// 获取加载数据库
	CMysqlHelper& GetLoadCMysqlHelper();
	// 获取条件变量
	ConditionVariable& GetConditionVariable();

public:
	// 网络消息回调
	bool CallBackFun(MsgCmd cmd, PlayerInfo* playerInfo);
	// 添加全局的网络回调
	void AddNetCallback(MsgCmd cmd, std::function<void(PlayerInfo*)>&& fun);

private:
	// 执行数据库语句线程
	void HandlerExecuteSqlThread();
	void HandleEexcuteMysql(std::string& sql);
	bool SwapMysqlList(ListString& LSqlList, ListString& RSqlList, bool& run);
	// 消息派发
	void MessageDispatch(MsgCmd cmd, PlayerInfo* playerInfo);

private:
	// 条件变量数据库用
	ConditionVariable m_cond;
	// 数据库语链表
	ListString        m_sqlList;	
	// 回调函数
	MapNetFun		  m_NetCBFunMap;
	// 数据库加载专用
	CMysqlHelper      m_CMysqlHelperLoad;
	// 数据库保存专用
	CMysqlHelper      m_CMysqlHelperSave;	
};