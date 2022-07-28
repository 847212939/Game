#pragma once

// 日志等级
enum LogLevel
{
	LOG_INFO = 0,							//info
	LOG_WARN,								//warnning
	LOG_ERROR,								//error
	LOG_CINFO,								//info Console tip
	LOG_CERROR,								//error Console
	LOG_ERROR_SYS,							//error system
	LOG_END,
};

const std::array<const char*, LOG_END> levelNames = { "[INF]", "[WAR]", "[ERR]", "[INF]","[ERR]", "[SYS]", };

#define LogMgr() CGameLogManage::Instance()
#define CfgMgr() CLuaMgr::Instance()->GetConfigMgr()
#define LuaMgr() CLuaMgr::Instance()

#define COUT_LOG(ERRTYPE, ...) \
if (ERRTYPE == LOG_ERROR) { CLog::Write(LogMgr()->GetErrorLog().c_str(), LOG_ERROR, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__); }\
else if (ERRTYPE == LOG_INFO) { CLog::Write(LogMgr()->GetErrorLog().c_str(), LOG_INFO, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__); }\
else if (ERRTYPE == LOG_WARN) { CLog::Write(LogMgr()->GetErrorLog().c_str(), LOG_WARN, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__); }\
else if (ERRTYPE == LOG_CERROR) { CLog::Write(LogMgr()->GetErrorLog().c_str(), LOG_CERROR, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__); }\
else if (ERRTYPE == LOG_CINFO) { CLog::Write(LogMgr()->GetErrorLog().c_str(), LOG_CINFO, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__); }

//计算数组维数
#define  CountArray(Array) (sizeof(Array)/sizeof(Array[0]))
//安全删除指针
#define  SafeDelete(pData) { try { delete pData; } catch (...) { CLog::Write(LogMgr()->GetErrorLog().c_str(), LOG_CERROR, __FILE__, __LINE__, __FUNCTION__, "DELETE ERR"); } pData = nullptr; } 
//安全删除指针
#define  SafeDeleteArray(pData)	{ if (pData) { try { delete[] pData;} catch (...) { CLog::Write(LogMgr()->GetErrorLog().c_str(), LOG_CERROR, __FILE__, __LINE__, __FUNCTION__, "DELETE ERR"); } } } 
//安全删除指针
#define  SAFE_DELETE(pData) { if(pData){ try{ delete pData; } catch(...){ CLog::Write(LogMgr()->GetErrorLog().c_str(), LOG_CERROR, __FILE__, __LINE__, __FUNCTION__, "DELETE ERR");  } pData = nullptr; } }
//判断大小函数
#define Min_(x,y) ((x)>(y)?(y):(x))
#define Max_(x,y) ((x)>(y)?(x):(y))

typedef std::map<unsigned int, std::function<void(void* pDataLineHead)>>		TypeFunMap;

// 注册客户端服务器消息回调
#define RegisterNetType(obj, name, cmd) obj->AddNetTypeCallback(cmd, std::move(std::bind(&name, obj, std::placeholders::_1)));
