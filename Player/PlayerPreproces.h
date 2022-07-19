#pragma once

// 玩家预处理
class PlayerPreproces
{
public:
	PlayerPreproces(TCPClient* pTCPClient);
	virtual ~PlayerPreproces();

public:
	void Init();
	void HandlerMessage(PlayerInfo* pPlayerInfo);

public:
	// 加载一条数据库
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
	// 定时器
	bool SetTimer(TimerCmd uTimerID, UINT uElapse, BYTE timerType = SERVERTIMER_TYPE_PERISIST);
	bool KillTimer(TimerCmd uTimerID);

public:
	bool CallBackFun(TimerCmd cmd);
	void DelTimerCallback(TimerCmd cmd);
	bool CallBackFun(MsgCmd cmd, PlayerInfo* pPlayerInfo);
	void AddTimerCallback(TimerCmd cmd, std::function<void()>&& fun);
	void AddNetCallback(MsgCmd cmd, std::function<void(PlayerInfo*)>&& fun);

public:
	// 创建角色
	void CreatePlayer(unsigned int index, const TCPSocketInfo* pSockInfo, std::string& id, std::string& pw);

private:
	bool InitDB();
	void DispatchMessage(MsgCmd cmd, PlayerInfo* pPlayerInfo);

public:
	static char createptable[CreateTableLen];
	static char createpptable[CreateTableLen];

private:
	TCPClient*        m_pTCPClient;			// 网络客户端
	SubScene          m_SubScene;			// 玩家场景
	CMysqlHelper      m_CMysqlHelper;		// 数据库
	NetFunMap		  m_NetCBFunMap;		// 回调函数
	CServerTimer*	  m_pServerTimer;		// 定时器
	TimerFunMap		  m_TimerFunMap;		// 回调函数
};
