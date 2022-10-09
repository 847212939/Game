#pragma once

// 玩家类
class Player : virtual public Animal
{
public:
	Player(const unsigned int& index);
	virtual ~Player();

public:
	virtual AnimalType GetType();

protected:
	Player(const Player& my);
	Player& operator=(const Player& my);

public:
	// 消息处理
	bool SendData(const char* pData, size_t size, MsgCmd mainID, int assistID, int handleCode, unsigned int uIdentification = 0);
	void MessageDispatch(MsgCmd cmd, PlayerInfo* playerInfo);

public:
	// 上下线处理
	void CalAttrs();
	void LoadMysql();
	void EnterScene();
	void ExitGame(SocketCloseLine* pSocketClose);

public:
	void AttrsCallBackFun();
	void MysqlCallBackFun();
	void EnterSceneCallBackFun();
	void ExitCallBackFun(SocketCloseLine* pSocketClose);
	void NetCallBackFun(MsgCmd cmd, PlayerInfo* playerInfo);
	void AddAttrsCallback(std::function<void()>&& fun);
	void AddEnterSceneCallback(std::function<void()>&& fun);
	void AddExitCallback(std::function<void(SocketCloseLine*)>&& fun);
	void AddNetCallback(MsgCmd cmd, std::function<void(PlayerInfo*)>&& fun);
	void AddMysqlCallback(std::string name, std::function<void(std::string&)>&& fun);

public:
	// 数据库操作
	void LoadOneSql(std::string sqlName, std::string& outStr, std::string dataStr = "data");
	void SaveInsertSQL(std::string sqlName, std::string data, std::string keyName = "userid", std::string dataName = "data");
	void SaveDeleteSQL(std::string sqlName, const std::string& sCondition);
	void SaveReplaceSQL(std::string sqlName, std::string data, std::string keyName = "userid", std::string dataName = "data");
	void SaveUpdateSQL(std::string sqlName, std::string data, const std::string& sCondition, std::string keyName = "userid", std::string dataName = "data");

public:
	GetSetMem(std::string, Playername)	// 玩家网名

	GetSetMem(bool, Load)				// 玩家是否初始化完成
	GetSetMem(unsigned int, Index)		// 玩家是否初始化完成

private:
	MapNetFun				m_NetCBFunMap;			// 回调函数集合
	VectorExitFunc			m_ExitFunMap;			// 回调函数集合
	MapMysqlFunc			m_MysqlCBFunMap;		// 回调函数集合
	VectorVoidFunc			m_AttrsFunMap;			// 回调函数集合
	VectorVoidFunc			m_EnterSceneFunMap;		// 回调函数集合
};