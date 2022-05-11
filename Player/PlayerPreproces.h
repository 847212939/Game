#pragma once

// 玩家预处理
class PlayerPreproces
{
	// 玩家账户信息
	typedef std::map<std::string, std::string> AccountMap;
	// 数据库语句list<sql>
	typedef std::list<std::string> SqlList;
public:
	PlayerPreproces(TCPClient* pTCPClient);
	virtual ~PlayerPreproces();

public:
	// 处理消息
	void HandlerMessage(PlayerInfo* pInfo);
	// insert mysql
	bool SaveInsertSQL(std::string sqlName, std::string name, std::string data, std::string keyName = "userid", std::string dataName = "data");
	// delete mysql
	bool SaveDeleteSQL(std::string sqlName, const std::string& sCondition);
	// replace mysql
	bool SaveReplaceSQL(std::string sqlName, std::string name, std::string data, std::string keyName= "userid", std::string dataName ="data");
	// update mysql
	bool SaveUpdateSQL(std::string sqlName, std::string name, std::string data, const std::string& sCondition, std::string keyName = "userid", std::string dataName = "data");

private:
	// 初始化DB
	bool InitDB();
	// 注册账号
	bool RegisterAccount(std::string& id, std::string& passwaed);
	// 检查账号信息
	bool CheckUserAccount(std::string& id, std::string& passwaed);
	// 加载玩家账号信息
	std::string LoadUserAccount(std::string& id);
	// 线程启动
	bool Run();
	// 数据库执行
	void HandlerExecuteDB();
	// 分发消息
	void DispatchMessage();
	// 创建角色
	bool CreatePlayr();
	// 获取通知条件变量
	ConditionVariable& GetConditionVariable();

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
	
};
