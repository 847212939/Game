#pragma once
#include <map>
#include <string>
#include <mutex>
#include <unordered_map>

namespace DRMU
{
	const int MAX_LOG_BUF_SIZE = 1024;		// log缓冲区大小
	const int MAX_FILE_NAME_SIZE = 256;		// 最大文件名长度
	const int MAX_FUNC_NAME_SIZE = 128;		// 最大函数名长度
	const int MAX_LOG_FILE_SIZE = 1024 * 1024 * 30;  //日子文件的最大的长度，超过长度会生成新的文件

	//多线程打印日志的时候还要再考虑下
	// 游戏log管理
	class CGameLogManage
	{
	private:
		CGameLogManage();
		virtual ~CGameLogManage();

	public:
		static CGameLogManage* Instance();

	public:
		void Release();
		// 设置服务器类型
		void SetLogFileType(int threadType);
		// 获取对应线程的errorlog
		std::string GetErrorLog();
		// 增加指定文件的fp
		bool AddLogFileFp(std::string strFile, FILE* fp);
		// 获取指定文件的fp
		FILE* GetLogFileFp(std::string&& strFile);
		// 设置进程日志目录
		void SetLogPath(const std::string& path);
		// 获取进程日志目录
		std::string GetLogPath();
		//获得锁
		std::mutex& GetMutex();

	private:
		std::mutex m_mutex;
		// 日志文件map
		std::map<int, std::string> m_LogFilesMap;
		// 文件描述符map
		std::unordered_map<std::string, FILE*> m_filesFpMap;
		// 进程日志目录
		std::string m_logPath;
		//服务器类型
		int m_serviceType;
	};

	//// 日志类
	class CLog
	{
	public:
		CLog() {}
		virtual ~CLog() {}

	public:
		static void Write(const char* pLogFile, int level, const char* pFile, int line, const char* pFuncName, const char* pFormat, ...);
		static void Write(const char* pLogFile, const char* pFuncName, const char* pFormat, ...);
		// 把buf中的内容写入logFile
		static void Write(const char* pLogFile, const char* buf);
		// 把buf中的内容写入logFile，文件名字用户自己定义
		static void Write(const char* pLogFile, const char* pFile, int line, const char* pFuncName, const char* pBuf, ...);
	};
}

