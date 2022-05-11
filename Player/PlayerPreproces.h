#pragma once

// 玩家预处理
class PlayerPreproces
{
public:
	// 注册登录子协议
	enum class PlayerPreprocesCmd
	{
		cs_register     = 1,  // 注册
		cs_login        = 2,  // 登录
	};
public:
	// 玩家账户信息
	typedef std::map<std::string, std::string> AccountMap;
	// 数据库语句list<sql>
	typedef std::list<std::string> SqlList;
	// 消息回调函数
	typedef std::map<MsgCmd, std::function<void(PlayerInfo*)>> CallBackFunMap;

public:
	PlayerPreproces(TCPClient* pTCPClient);
	virtual ~PlayerPreproces();

public:
	// 处理消息
	void HandlerMessage(PlayerInfo* pPlayerInfo);
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
	// 获取玩家账户信息
	AccountMap& GetAccountMap();
	// 获取网络句柄
	TCPClient* GetTCPClient();
	// 获取数据库
	CMysqlHelper& GetCMysqlHelper();
	// 获取玩家管理
	PlayerCenter& GetPlayerCenter();
	// 获取回调函数map
	CallBackFunMap& GetCallBackFunMap();
	// 加入回调函数
	void AddCallBackFun(MsgCmd cmd, std::function<void(PlayerInfo*)>&& fun);
	// 回调函数
	bool CallBackFun(MsgCmd cmd, PlayerInfo* pPlayerInfo);

private:
	// 初始化消息回调函数
	bool intCallBackFun();
	// 初始化DB
	bool InitDB();
	// 线程启动
	bool Run();
	// 数据库执行
	void HandlerExecuteDB();
	// 分发消息
	void DispatchMessage(MsgCmd cmd, PlayerInfo* pPlayerInfo);
	// 注册账号
	void RegisterAccount(PlayerInfo* pPlayerInfo);
	// 检查账号信息
	bool CheckUserAccount(std::string& id, std::string& passwaed);
	// 加载玩家账号信息
	std::string LoadUserAccount(std::string& id);
	// 创建角色
	bool CreatePlayr();

private:
	// 条件变量数据库用
	ConditionVariable m_cond;
	// 数据库语链表
	SqlList			m_sqlList;
	// 网络
	TCPClient*		m_pTCPClient;
	// 玩家中心管理器
	PlayerCenter	m_PlayerCenter;
	// 数据库
	CMysqlHelper	m_CMysqlHelper;
	//玩家账户信息
	AccountMap		m_accountMap;
	// 回调函数
	CallBackFunMap	m_CallBackFunMap;
	
};
