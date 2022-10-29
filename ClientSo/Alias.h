#pragma once

typedef void (*NetworkCallBackFunc)(REvent eve/*, char m_Source[]*/);
typedef void (*TimerCallBackFunc)(int timer);
typedef void (*CloseCallBackFunc)();

template<typename T>
using SetTemplate			= std::set<T>;
using SetUint				= std::set<unsigned int>;

template<typename T>
using VectorTemplate		= std::vector<T>;
using VectorInt				= std::vector<int>;
using VectorThread			= std::vector<std::thread*>;
using VectorTCPSocketInfo	= std::vector<TCPSocketInfo>;
using VectorWorkThreadInfo	= std::vector<WorkThreadInfo>;
using VectorString			= std::vector<std::string>;
using VectorVoidFunc		= std::vector<std::function<void()>>;
using VectorExitFunc		= std::vector<std::function<void(SocketCloseLine*)>>;

using ListUint				= std::list<unsigned int>;
using ListString			= std::list<std::string>;

template<typename T>
using MapIntVecTemplate		= std::map<int, VectorTemplate<T>>;
using MapStringString		= std::map<std::string, std::string>;
using MapNetFun				= std::map<MsgCmd, std::function<void(PlayerInfo*)>>;
using MapMysqlFunc			= std::map<std::string, std::function<void(std::string&)>>;
using MapTypeFunc			= std::map<SysMsgCmd, std::function<void(void* pDataLineHead)>>;
using MapRecordData			= std::map<std::string, std::pair<FT, std::string>>;
using MapServerId			= std::map<uint64_t, std::set<int>>;
using MapServerTimerInfo	= std::unordered_map<unsigned int, ServerTimerInfo>;
using MapTimerFunc			= std::map<TimerCmd, TimerCallBackFunc>;

using LogLevelNames			= const std::array<const char*, LOG_END>;
LogLevelNames levelNames	= { "[INF]", "[WAR]", "[ERR]", "[INF]","[ERR]", "[SYS]", };

