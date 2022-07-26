#pragma once

// 玩家信息
class PlayerInfo
{
public:
	PlayerInfo();
	virtual~PlayerInfo();

public:
	
	SocketReadLine* m_pMsg;					// SOCKET读取通知结构定义
	void*			m_pData;				// 玩家发送过来的数据
	ServiceType		m_uSrverType;			// 服务器类型
	uint64_t		m_userId;				// 玩家id
	const TCPSocketInfo* m_pTcpSockInfo;	// 玩家TCP的网络信息
};

struct LoadPlayerKey
{
	unsigned int			index;
	uint64_t				userId;
	const TCPSocketInfo*	pSockInfo;
	std::string				id;
	std::string				pw;

	bool GetConnect();
	const unsigned int& GetIndex();
	const TCPSocketInfo* GetSocketInfo();
	const uint64_t& getUserId();

	LoadPlayerKey(int nIndex, const TCPSocketInfo* sockInfo, std::string& sId, std::string& sPw) : index(nIndex), pSockInfo(sockInfo), userId(0), id(sId), pw(sPw) {}
	~LoadPlayerKey() {}
};

typedef std::list<UINT>												TimerList;		// 定时器
typedef std::map<int, int>											AttrsMap;		// 属性
typedef std::set<unsigned int>										OnLinePlayerSet;// 在线玩家
typedef std::list<std::string>										SqlList;		// 数据库语句list<sql>	
typedef std::map<std::string, std::string>							SqlKeyDataMap;	// 数据库查询结果
typedef std::vector<std::function<void()>>							AttrsFunMap;	// 消息回调函数
typedef std::map<TimerCmd, std::function<void()>>					TimerFunMap;	// 消息回调函数
typedef std::vector<std::function<void(SocketCloseLine*)>>			ExitFunMap;		// 消息回调函数
typedef std::map<MsgCmd, std::function<void(PlayerInfo*)>>			NetFunMap;		// 消息回调函数
typedef std::map<std::string, std::function<void(std::string&)>>	MysqlFunMap;	// 消息回调函数

// 注册游戏进入回调
#define RegisterAttrs(pobj, obj, name)\
if (!pobj)\
{\
	CLog::Write(LogMgr()->GetErrorLog().c_str(), LOG_CERROR, __FILE__, __LINE__, __FUNCTION__, "注册消息失败 请检查写法");\
}\
else\
{\
	pobj->AddAttrsCallback(std::move(std::bind(&name, obj)));\
}

// 注册网络协议
#define RegisterNetwk(pobj, obj, name, cmd)\
if (!pobj)\
{\
	CLog::Write(LogMgr()->GetErrorLog().c_str(), LOG_CERROR, __FILE__, __LINE__, __FUNCTION__, "注册消息失败 请检查写法");\
}\
else\
{\
	pobj->AddNetCallback(cmd, std::move(std::bind(&name, obj, std::placeholders::_1)));\
}

// 注册数据库回调
#define RegisterMysql(pobj, obj, name, sql)\
if (!pobj)\
{\
	CLog::Write(LogMgr()->GetErrorLog().c_str(), LOG_CERROR, __FILE__, __LINE__, __FUNCTION__, "注册消息失败 请检查写法");\
}\
else\
{\
	pobj->AddMysqlCallback(sql, std::move(std::bind(&name, obj, std::placeholders::_1)));\
}

// 注册创建数据库
#define RegisterCreat(pobj, name)\
if (!pobj)\
{\
	CLog::Write(LogMgr()->GetErrorLog().c_str(), LOG_CERROR, __FILE__, __LINE__, __FUNCTION__, "注册消息失败 请检查写法");\
}\
else\
{\
	pobj->CreateTableI(name);\
}

#define RegisterCreatS(pobj, name)\
if (!pobj)\
{\
	CLog::Write(LogMgr()->GetErrorLog().c_str(), LOG_CERROR, __FILE__, __LINE__, __FUNCTION__, "注册消息失败 请检查写法");\
}\
else\
{\
	pobj->CreateTableS(name);\
}

// 注册定时器
#define RegisterTimer(pobj, obj, name, cmd, uElapse, timerType)\
if (!pobj)\
{\
	CLog::Write(LogMgr()->GetErrorLog().c_str(), LOG_CERROR, __FILE__, __LINE__, __FUNCTION__, "注册消息失败 请检查写法"); \
}\
else\
{\
	pobj->SetTimer(cmd, uElapse, timerType);\
	pobj->AddTimerCallback(cmd, std::move(std::bind(&name, obj)));\
}

// 反注册定时器
#define UnRegisterTimer(pobj, cmd)\
if (!pobj)\
{\
	CLog::Write(LogMgr()->GetErrorLog().c_str(), LOG_CERROR, __FILE__, __LINE__, __FUNCTION__, "注册消息失败 请检查写法"); \
}\
else\
{\
	pobj->KillTimer(cmd);\
	pobj->DelTimerCallback(cmd);\
}

// 添加属性
#define AddAttributes(pobj, attrs)\
if (!pobj)\
{\
	CLog::Write(LogMgr()->GetErrorLog().c_str(), LOG_CERROR, __FILE__, __LINE__, __FUNCTION__, "添加属性失败");\
}\
else\
{\
	pobj->AdditionAttributes(attrs);\
}

// 注册游戏退出
#define RegisterLgout(pobj, obj, name)\
if (!pobj)\
{\
	CLog::Write(LogMgr()->GetErrorLog().c_str(), LOG_CERROR, __FILE__, __LINE__, __FUNCTION__, "注册消息失败 请检查写法");\
}\
else\
{\
	pobj->AddExitCallback(std::move(std::bind(&name, obj, std::placeholders::_1)));\
}
