#pragma once

// 玩家类
class Player
{
public:
	Player(const unsigned int& index, const uint64_t& userId);
	virtual ~Player();

protected:
	Player(const Player& my);
	Player& operator=(const Player& my);

public:
	bool GetLoad();
	int GetIndex();
	uint64_t GetUserId() const;

public:
	// 消息处理
	bool SendData(const char* pData, size_t size, MsgCmd mainID, int assistID, int handleCode, unsigned int uIdentification = 0);
	void MessageDispatch(MsgCmd cmd, PlayerInfo* playerInfo);

public:
	// 上下线处理
	void LoadMysql();
	void EnterGame();
	bool EnterScene();
	void ExitGame(SocketCloseLine* pSocketClose);
	void SetLoad(bool load);

public:
	void AddAttrsCallback(std::function<void()>&& fun);
	void AddExitCallback(std::function<void(SocketCloseLine*)>&& fun);
	void AddNetCallback(MsgCmd cmd, std::function<void(PlayerInfo*)>&& fun);
	void AddMysqlCallback(std::string name, std::function<void(std::string&)>&& fun);
	void AttrsCallBackFun();
	void MysqlCallBackFun();
	void ExitCallBackFun(SocketCloseLine* pSocketClose);
	void NetCallBackFun(MsgCmd cmd, PlayerInfo* playerInfo);

public:
	// 属性
	void AdditionAttributes(AttrsMap& attrs);
	void RefreshProperties();

public:
	// 数据库操作
	void LoadOneSql(std::string sqlName, std::string& outStr, std::string dataStr = "data");
	void SaveInsertSQL(std::string sqlName, std::string data, std::string keyName = "userid", std::string dataName = "data");
	void SaveDeleteSQL(std::string sqlName, const std::string& sCondition);
	void SaveReplaceSQL(std::string sqlName, std::string data, std::string keyName = "userid", std::string dataName = "data");
	void SaveUpdateSQL(std::string sqlName, std::string data, const std::string& sCondition, std::string keyName = "userid", std::string dataName = "data");

private:
	bool					m_load;					// 玩家是否初始化完成
	unsigned int			m_index;				// 玩家索引
	uint64_t				m_userId;				// 玩家id
	AttrsMap				m_AttrsMap;				// 属性集合
	NetFunMap				m_NetCBFunMap;			// 回调函数集合
	ExitFunMap				m_ExitFunMap;			// 回调函数集合
	AttrsFunMap				m_AttrsFunMap;			// 回调函数集合
	MysqlFunMap				m_MysqlCBFunMap;		// 回调函数集合
};