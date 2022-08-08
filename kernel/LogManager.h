#pragma once

class CGameLogManage
{
private:
	CGameLogManage();
	virtual ~CGameLogManage();

protected:
	CGameLogManage(const CGameLogManage& my);
	CGameLogManage& operator=(const CGameLogManage& my);

public:
	static CGameLogManage* Instance();

public:
	void Release();
	// 设置服务器类型
	void SetLogFileType(ServiceType serviceType);
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
	// 获得锁
	std::mutex& GetMutex();
	// 日志缓存
	std::list<std::pair<FILE*, std::string>>& GetLogMap();
	// 日志打印
	void Fflush(char* logBuf);
	void Init(bool& run);

private:
	// 日志处理线程
	void HandlerLogThread(bool& run);
	int GetLogHour(const std::string& str);

private:
	std::mutex m_mutex;
	// 日志文件map
	std::map<ServiceType, std::string> m_LogFilesMap;
	// 文件描述符map
	std::unordered_map<std::string, FILE*> m_filesFpMap;
	// 进程日志目录
	std::string m_logPath;
	//服务器类型
	ServiceType m_serviceType;
	// 日志缓存
	std::list<std::pair<FILE*, std::string>> m_logMap;
	// 日志线程
	std::vector<std::thread*> m_threadVec;
};

//// 日志类
class CLog
{
public:
	CLog() {}
	virtual ~CLog() {}

public:
	static void Write(const char* pLogFile, int level, const char* pFile, int line, const char* pFuncName, const char* pFormat, ...);
};
