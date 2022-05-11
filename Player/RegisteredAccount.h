#pragma once

class RegisteredAccount
{
	// 数据库语句unordered_map<tableName, sql>
	typedef std::unordered_map<std::string, std::string> SqlMap;
public:
	RegisteredAccount(TCPClient* pTCPClient);
	virtual ~RegisteredAccount();

public:
	// 启动数据库
	bool InitDB();
	//线程启动
	bool Run();
	//	处理消息
	void HandelMessage(PlayerInfo* pInfo);
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
	TCPClient*		m_TCPClient;
	// 玩家中心管理器
	PlayerCenter	m_PlayerCenter;
	// 数据库
	CMysqlHelper	m_CMysqlHelper;
	// 条件变量数据库用
	ConditionVariable m_cond;
	
};
