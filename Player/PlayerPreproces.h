#pragma once

// 玩家预处理
class PlayerPreproces
{
public:
	// 注册登录子协议
	enum class PlayerPreprocesCmd
	{
		cs_login        = 1,  // 登录
	};
public:
	typedef std::list<std::string> SqlList;										// 数据库语句list<sql>	
	typedef std::map<MsgCmd, std::function<void(PlayerInfo*)>> CallBackFunMap;	// 消息回调函数
	typedef std::map<std::string, std::string> SqlKeyDataMap;					// 数据库查询结果

public:
	PlayerPreproces(TCPClient* pTCPClient);
	virtual ~PlayerPreproces();

public:
	// 处理消息
	void HandlerMessage(PlayerInfo* pPlayerInfo);

	// 加载一条数据库
	bool LoadOneSql(std::string userId, std::string sqlName, CMysqlHelper::MysqlData& queryData, std::string dataStr = "data");
	// 加载多条数据库
	bool LoadMulitySql(std::string userId, std::string sqlName, CMysqlHelper::MysqlData& queryData, std::string dataStr = "data");
	// insert mysql
	void SaveInsertSQL(std::string sqlName, std::string name, std::string data, std::string keyName = "userid", std::string dataName = "data");
	// delete mysql
	void SaveDeleteSQL(std::string sqlName, const std::string& sCondition);
	// replace mysql
	void SaveReplaceSQL(std::string sqlName, std::string name, std::string data, std::string keyName= "userid", std::string dataName ="data");
	// update mysql
	void SaveUpdateSQL(std::string sqlName, std::string name, std::string data, const std::string& sCondition, std::string keyName = "userid", std::string dataName = "data");
	
	// 获取通知条件变量
	ConditionVariable& GetConditionVariable();
	// 获取网络句柄
	TCPClient* GetTCPClient();
	// 获取数据库
	CMysqlHelper& GetCMysqlHelper();
	// 获取场景
	Scene& GetScene();
	// 获取回调函数map
	CallBackFunMap& GetCallBackFunMap();
	// 加入回调函数
	void AddCallBackFun(MsgCmd cmd, std::function<void(PlayerInfo*)>&& fun);
	// 回调函数
	bool CallBackFun(MsgCmd cmd, PlayerInfo* pPlayerInfo);

private:
	// 初始化消息回调函数
	void initCallBackFun();
	// 初始化DB
	bool InitDB();
	// 线程启动
	bool Run();

	// 数据库执行
	void HandlerExecuteSqlThread();
	// 分发消息
	void DispatchMessage(MsgCmd cmd, PlayerInfo* pPlayerInfo);

	// 登录
	void LoginInAccount(PlayerInfo* pPlayerInfo);
	// 登录
	bool LoginIn(std::string& id, std::string& passwaed, PlayerInfo* pPlayerInfo);

	// 加载玩家账号信息
	std::string LoadUserAccount(std::string& id);
	// 加载玩家userid
	std::string LoadUserId(std::string& id);

	// 创建角色
	bool CreatePlayer(unsigned int index, const TCPSocketInfo* pSockInfo, std::string& userId);

private:
	ConditionVariable m_cond;				// 条件变量数据库用
	SqlList           m_sqlList;			// 数据库语链表
	TCPClient*        m_pTCPClient;			// 网络客户端
	Scene             m_scene;				// 玩家场景
	CMysqlHelper      m_CMysqlHelper;		// 数据库
	CallBackFunMap    m_CallBackFunMap;		// 回调函数
};
