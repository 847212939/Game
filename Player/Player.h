#pragma once

// 玩家类
class Player
{
public:
	typedef std::map<MsgCmd, std::function<void(PlayerInfo*)>> PlayerCallBackFunMap;
public:
	Player(const unsigned int& index, const TCPSocketInfo* pSockInfo, const std::string& userId);
	virtual ~Player();

public:
	std::string GetUserId() const;
	int GetIndex() { return m_index; }
	const TCPSocketInfo* GetTCPSocketInfo();

public:
	// 消息处理
	bool SendData(int index, void* pData, int size, int mainID, int assistID, int handleCode, void* pBufferevent, unsigned int uIdentification = 0);
	void DispatchMessage(MsgCmd cmd, PlayerInfo* pPlayerInfo);

public:
	// 玩家上线
	void LoadMysql();						
	void EnterGame();						
	bool EnterScene();
	void SetPlayerPreproces(SubPlayerPreproces* pp) { m_SubPlayerPreproces = pp; }

public:
	// 回调函数
	void AddCallBackFun(MsgCmd cmd, std::function<void(PlayerInfo*)>&& fun);
	bool CallBackFun(MsgCmd cmd, PlayerInfo* pPlayerInfo);

public:
	// 数据库操作
	// 加载一条数据库
	bool LoadOneSql(std::string userId, std::string sqlName, CMysqlHelper::MysqlData& queryData, std::string dataStr = "data");
	// 加载多条数据库
	bool LoadMulitySql(std::string userId, std::string sqlName, CMysqlHelper::MysqlData& queryData, std::string dataStr = "data");
	// insert mysql
	void SaveInsertSQL(std::string sqlName, std::string name, std::string data, std::string keyName = "userid", std::string dataName = "data");
	// delete mysql
	void SaveDeleteSQL(std::string sqlName, const std::string& sCondition);
	// replace mysql
	void SaveReplaceSQL(std::string sqlName, std::string name, std::string data, std::string keyName = "userid", std::string dataName = "data");
	// update mysql
	void SaveUpdateSQL(std::string sqlName, std::string name, std::string data, const std::string& sCondition, std::string keyName = "userid", std::string dataName = "data");

private:
	unsigned int			m_index;			// 玩家索引
	PlayerAttrs				m_PlayerAttrs;		// 玩家属性
	std::string				m_userId;			// 玩家id
	const TCPSocketInfo*	m_pTcpSockInfo;		// 玩家TCP的网络信息
	SubPlayerPreproces*		m_SubPlayerPreproces;	// 玩家预处理句柄
	PlayerCallBackFunMap	m_CallBackFunMap;	// 回调函数
};