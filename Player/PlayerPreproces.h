#pragma once

// 玩家预处理
class PlayerPreproces
{
public:
	typedef std::list<std::string> SqlList;										// 数据库语句list<sql>	

public:
	PlayerPreproces(TCPClient* pTCPClient);
	virtual ~PlayerPreproces();

public:
	void Init();

public:
	// 处理消息
	void HandlerMessage(PlayerInfo* pPlayerInfo);

public:
	// 加载一条数据库
	std::string LoadOneSql(std::string userId, std::string sqlName, std::string dataStr = "data");
	std::string LoadOneSql(std::string sqlName, uint64_t userId, std::string dataStr = "data");
	// 加载多条数据库
	bool LoadMulitySql(std::string sqlName, uint64_t userId, CMysqlHelper::MysqlData& queryData, std::string dataStr = "data");
	// create table
	void CreateTableSql(const char* sql);
	// insert mysql
	void SaveInsertSQL(std::string sqlName, uint64_t userId, std::string data, std::string keyName = "userid", std::string dataName = "data");
	// delete mysql
	void SaveDeleteSQL(std::string sqlName, const std::string& sCondition);
	// replace mysql
	void SaveReplaceSQL(std::string sqlName, uint64_t userId, std::string data, std::string keyName = "userid", std::string dataName = "data");
	void SaveReplaceSQL(std::string sqlName, std::string userId, std::string data, std::string keyName = "userid", std::string dataName = "data");
	// update mysql
	void SaveUpdateSQL(std::string sqlName, uint64_t userId, std::string data, const std::string& sCondition, std::string keyName = "userid", std::string dataName = "data");
	
public:
	SubScene& GetSubScene();
	TCPClient* GetTCPClient();
	CMysqlHelper& GetCMysqlHelper();
	ConditionVariable& GetConditionVariable();

public:
	void AddNetCallback(MsgCmd cmd, std::function<void(PlayerInfo*)>&& fun);
	bool CallBackFun(MsgCmd cmd, PlayerInfo* pPlayerInfo);

public:
	// 创建角色
	void CreatePlayer(unsigned int index, const TCPSocketInfo* pSockInfo, std::string& id, std::string pw);

private:
	// 初始化DB
	bool InitDB();
	// 线程启动
	bool RunThread();

private:
	// 数据库执行
	void HandlerExecuteSqlThread();
	// 分发消息
	void DispatchMessage(MsgCmd cmd, PlayerInfo* pPlayerInfo);

private:
	ConditionVariable m_cond;				// 条件变量数据库用
	SqlList           m_sqlList;			// 数据库语链表
	TCPClient*        m_pTCPClient;			// 网络客户端
	SubScene          m_SubScene;			// 玩家场景
	CMysqlHelper      m_CMysqlHelper;		// 数据库
	NetFunMap		  m_NetCBFunMap;		// 回调函数
};
