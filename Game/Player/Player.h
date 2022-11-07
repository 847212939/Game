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
	// 给一个玩家发送消息
	bool SendMsg(const char* pData, size_t size, MsgCmd mainID, int assistID, MsgCmd uIdentification = MsgCmd::MsgCmd_Begin, int handleCode = 0);
	// 消息派发
	void MessageDispatch(MsgCmd cmd, PlayerInfo* playerInfo);
private:
	bool SendLogicMsg(const char* pData, size_t size, MsgCmd mainID, int assistID, int handleCode, unsigned int uIdentification = 0);
	bool SendCrossMsg(const char* pData, size_t size, MsgCmd mainID, int assistID, int handleCode, unsigned int uIdentification = 0);

public:
	// 上下线处理
	// 上线属性计算
	void CalAttrs();
	// 上线加载数据库
	void LoadMysql();
	// 上线进入场景
	void EnterScene();
	// 下线退出游戏
	void ExitGame(SocketCloseLine* pSocketClose);

public:
	// 属性回调函数
	void AttrsCallBackFun();
	// 数据库回调函数
	void MysqlCallBackFun();
	// 进入场景回调函数
	void EnterSceneCallBackFun();
	// 退出游戏回调函数
	void ExitCallBackFun(SocketCloseLine* pSocketClose);
	// 网络消息回调函数
	void NetCallBackFun(MsgCmd cmd, PlayerInfo* playerInfo);
	// 添加属性回调函数
	void AddAttrsCallback(std::function<void()>&& fun);
	// 添加进入场景回调函数
	void AddEnterSceneCallback(std::function<void()>&& fun);
	// 添加退出游戏回调函数
	void AddExitCallback(std::function<void(SocketCloseLine*)>&& fun);
	// 添加网络消息回调函数
	void AddNetCallback(MsgCmd cmd, std::function<void(PlayerInfo*)>&& fun);
	// 添加数据库回调函数
	void AddMysqlCallback(SLoadMysql loadMysql);

public:
	// 玩家网名
	BuildValue(std::string, Playername)
	// 玩家是否初始化完成
	BuildValue(bool, Load)
	// 玩家是否初始化完成
	BuildValue(unsigned int, Index)
	// 玩家逻辑服的索引
	BuildValue(unsigned int, LogicIndex)
	// serverid
	BuildValue(int, Serverid)

private:
	// 回调函数集合
	MapNetFun				m_NetCBFunMap;	
	// 回调函数集合
	VectorExitFunc			m_ExitFunMap;	
	// 回调函数集合
	MapMysqlFunc			m_MysqlCBFunMap;
	// 回调函数集合
	VectorVoidFunc			m_AttrsFunMap;	
	// 回调函数集合
	VectorVoidFunc			m_EnterSceneFunMap;		
};