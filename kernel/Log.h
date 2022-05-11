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

#define LogManager() CGameLogManage::Instance()

#define COUT_LOG(ERRTYPE, ...) \
if (ERRTYPE == LOG_ERROR) { CLog::Write(LogManager()->GetErrorLog().c_str(), LOG_ERROR, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__); }\
else if (ERRTYPE == LOG_INFO) { CLog::Write(LogManager()->GetErrorLog().c_str(), LOG_INFO, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__); }\
else if (ERRTYPE == LOG_WARN) { CLog::Write(LogManager()->GetErrorLog().c_str(), LOG_WARN, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__); }\
else if (ERRTYPE == LOG_CERROR) { CLog::Write(LogManager()->GetErrorLog().c_str(), LOG_CERROR, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__); }\
else if (ERRTYPE == LOG_CINFO) { CLog::Write(LogManager()->GetErrorLog().c_str(), LOG_CINFO, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__); }

// 普通日志输出，文件名用户自定义
#define FILE_LOG(filename, ...)	{ CLog::Write(filename, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__); }

//计算数组维数
#define  CountArray(Array) (sizeof(Array)/sizeof(Array[0]))
//安全删除指针
#define  SafeDelete(pData) { try { delete pData; } catch (...) { CLog::Write(LogManager()->GetErrorLog().c_str(), LOG_ERROR, __FILE__, __LINE__, __FUNCTION__, "DELETE ERR"); } pData = NULL; } 
//安全删除指针
#define  SafeDeleteArray(pData)	{ if (pData) { try { delete[] pData;} catch (...) { CLog::Write(LogManager()->GetErrorLog().c_str(), LOG_ERROR, __FILE__, __LINE__, __FUNCTION__, "DELETE ERR"); } } } 
//安全删除指针
#define  SAFE_DELETE(pData) { if(pData){ try{ delete pData; } catch(...){ CLog::Write(LogManager()->GetErrorLog().c_str(), LOG_ERROR, __FILE__, __LINE__, __FUNCTION__, "DELETE ERR");  } pData = nullptr; } }
//判断大小函数
#define Min_(x,y) ((x)>(y)?(y):(x))
#define Max_(x,y) ((x)>(y)?(x):(y))
