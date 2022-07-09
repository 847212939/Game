#pragma once

// 玩家类
class Player
{
public:
	Player(const unsigned int& index, const TCPSocketInfo* pSockInfo, const uint64_t& userId);
	virtual ~Player();

public:
	bool GetLoad();
	int GetIndex();
	uint64_t GetUserId() const;
	const TCPSocketInfo* GetTCPSocketInfo();

public:
	// 消息处理
	bool SendData(int index, void* pData, int size, int mainID, int assistID, int handleCode, void* pBufferevent, unsigned int uIdentification = 0);
	void DispatchMessage(MsgCmd cmd, PlayerInfo* pPlayerInfo);

public:
	// 上线处理
	void LoadMysql();
	void EnterGame();
	bool EnterScene();

public:
	// 下线处理
	void ExitGame();

public:
	void SetPlayerPreproces(SubPlayerPreproces* pp);
	void SetLoad(bool load);

public:
	// 回调函数
	void AddNetCallback(MsgCmd cmd, std::function<void(PlayerInfo*)>&& fun);
	void AddMysqlCallback(std::string name, std::function<void(std::string&&)>&& fun);
	bool CallBackFun();
	bool CallBackFun(MsgCmd cmd, PlayerInfo* pPlayerInfo);

public:
	// 数据库操作
	// 加载一条数据库
	std::string LoadOneSql(std::string sqlName, uint64_t userId, std::string dataStr = "data");
	// insert mysql
	void SaveInsertSQL(std::string sqlName, uint64_t userId, std::string data, std::string keyName = "userid", std::string dataName = "data");
	// delete mysql
	void SaveDeleteSQL(std::string sqlName, const std::string& sCondition);
	// replace mysql
	void SaveReplaceSQL(std::string sqlName, uint64_t userId, std::string data, std::string keyName = "userid", std::string dataName = "data");
	// update mysql
	void SaveUpdateSQL(std::string sqlName, uint64_t userId, std::string data, const std::string& sCondition, std::string keyName = "userid", std::string dataName = "data");

private:
	bool					m_loadMysqled;			// 数据库加载完成标志
	unsigned int			m_index;				// 玩家索引
	PlayerAttrs				m_PlayerAttrs;			// 玩家属性
	uint64_t				m_userId;				// 玩家id
	const TCPSocketInfo*	m_pTcpSockInfo;			// 玩家TCP的网络信息
	SubPlayerPreproces*		m_SubPlayerPreproces;	// 玩家预处理句柄
	NetFunMap				m_NetCBFunMap;			// 回调函数集合
	MysqlFunMap				m_MysqlCBFunMap;		// 回调函数集合
};