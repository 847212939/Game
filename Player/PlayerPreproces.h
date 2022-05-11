#pragma once

// 玩家预处理
class PlayerPreproces
{
	// 数据库语句unordered_map<tableName, sql>
	typedef std::unordered_map<std::string, std::string> SqlMap;
public:
	PlayerPreproces(TCPClient* pTCPClient);
	virtual ~PlayerPreproces();

public:
	// 初始化DB
	bool InitDB();
	// 加载玩家账号信息
	bool LoadUserAccount();
	// 线程启动
	bool Run();
	// 数据库执行
	void HandlerExecuteDB();
	// 处理消息
	void HandlerMessage(PlayerInfo* pInfo);
	// 分发消息
	void DispatchMessage();
	// 创建角色
	bool CreatePlayr();
	// 获取通知条件变量
	ConditionVariable& GetConditionVariable();
private:
	// 数据库语链表
	SqlMap			m_sqlMap;
	// 网络
	TCPClient*		m_pTCPClient;
	// 玩家中心管理器
	PlayerCenter	m_PlayerCenter;
	// 数据库
	CMysqlHelper	m_CMysqlHelper;
	// 条件变量数据库用
	ConditionVariable m_cond;
	
};
