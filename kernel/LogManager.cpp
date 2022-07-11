#define _CRT_SECURE_NO_WARNINGS
#include "../Game/stdafx.h"

// 调试中用 线上注释掉

void CLog::Write(const char* pLogfile, int level, const char* pFile, int line, const char* pFuncName, const char* pBuf, ...)
{
	if (!pLogfile || !pFile || !pFuncName || !pBuf)
	{
		return;
	}
	if (level >= (int)levelNames.size())
	{
		return;
	}
	const char* levelName = levelNames[level];
	if (!levelName)
	{
		return;
	}
	char buf[MAX_LOG_BUF_SIZE] = "";

	// 时间
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);

	sprintf(buf, "[%04d-%02d-%02d %02d:%02d:%02d]", sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	// 线程ID和level
	unsigned long threadID = GetCurrentThreadId();
	sprintf(buf + strlen(buf), "%s[%05lu] ", levelName, threadID);

	// 参数
	va_list args;
	va_start(args, pBuf);

	vsprintf(buf + strlen(buf), pBuf, args);
	va_end(args);

#ifdef _DEBUG

	// 输出到控制台
	if (level == LOG_CINFO)
	{
		std::cout << buf << std::endl;
	}
	else if (level == LOG_CERROR)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		std::cout << buf << std::endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	}

#endif

	sprintf(buf + strlen(buf), " {%s %s %d}\n", pFile, pFuncName, line);

	std::string strPath = LogMgr()->GetLogPath() + pLogfile;

	FILE* fp = LogMgr()->GetLogFileFp(std::move(pLogfile));
	if (!fp)
	{
		fp = fopen(strPath.c_str(), "a+");
		if (!fp)
		{
			return;
		}
		LogMgr()->AddLogFileFp(pLogfile, fp);
	}

	std::multimap<FILE*, std::string>& logMap = LogMgr()->GetLogMap();
	std::lock_guard<std::mutex> guard(LogMgr()->GetMutex());
	logMap.insert(std::make_pair(fp, buf));
}

CGameLogManage::CGameLogManage()
{
	char filePath[512] = "";
	GetCurrentDirectoryA(512, filePath);
	std::string strPth = filePath;
	strPth += "\\log\\";

	m_logPath = strPth;
	m_serviceType = (ServiceType)0;

	m_threadVec.push_back(new std::thread(&CGameLogManage::HandlerLogThread, this));
}

CGameLogManage::~CGameLogManage()
{
	//Release();
	while (!m_threadVec.empty())
	{
		std::vector<std::thread*>::reverse_iterator it = m_threadVec.rbegin();
		(*it)->join();
		SafeDelete(*it);
	}
}

CGameLogManage* CGameLogManage::Instance()
{
	static CGameLogManage g_mgr;
	return &g_mgr;
}

void CGameLogManage::Release()
{
	for (auto iter = m_filesFpMap.begin(); iter != m_filesFpMap.end(); ++iter)
	{
		FILE* fp = iter->second;
		if (fp)
		{
			fclose(fp);
			fp = NULL;
		}
	}

	m_filesFpMap.clear();
}

void CGameLogManage::SetLogFileType(ServiceType serviceType)
{
	if (serviceType <= ServiceType::SERVICE_TYPE_BEGIN || serviceType >= ServiceType::SERVICE_TYPE_END)
	{
		return;
	}
	std::string nameStr;
	if (serviceType == ServiceType::SERVICE_TYPE_LOGIC)
	{
		nameStr += "LogicServer";
	}
	else if (serviceType == ServiceType::SERVICE_TYPE_DB)
	{
		nameStr += "DBServer";
	}
	else if (serviceType == ServiceType::SERVICE_TYPE_GATA)
	{
		nameStr += "GataServer";
	}
	else if (serviceType == ServiceType::SERVICE_TYPE_GAMECENTER)
	{
		nameStr += "GameCenterServer";
	}

	m_serviceType = serviceType;
	m_LogFilesMap.emplace(serviceType, nameStr);
}

std::string CGameLogManage::GetErrorLog()
{
	if (m_serviceType <= ServiceType::SERVICE_TYPE_BEGIN || m_serviceType >= ServiceType::SERVICE_TYPE_END)
	{
		return "";
	}
	std::string str = "";
	auto iter = m_LogFilesMap.find(m_serviceType);
	if (iter != m_LogFilesMap.end())
	{
		str = iter->second;
	}
	// 根据日期生成最终的文件
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);

	char buf[128] = "";
	sprintf(buf, "_%04d-%02d-%02d_%02d", sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour);
	str += buf;

	// 判断当前文件大小，超过重新生成文件
	struct stat statbuf;
	if (stat(str.c_str(), &statbuf) == 0 && statbuf.st_size > MAX_LOG_FILE_SIZE)
	{
		sprintf(buf, "-%02d", sysTime.wMinute);
	}
	str += ".txt";

	return str;
}

bool CGameLogManage::AddLogFileFp(std::string strFile, FILE* fp)
{

	if (strFile == "" || !fp)
	{
		return false;
	}
	if (m_filesFpMap.find(strFile) != m_filesFpMap.end())
	{
		return false;
	}
	std::lock_guard<std::mutex> guard(GetMutex());
	m_filesFpMap.emplace(strFile, fp);

	return true;
}

FILE* CGameLogManage::GetLogFileFp(std::string&& strFile)
{
	if (strFile == "")
	{
		return NULL;
	}

	auto iter = m_filesFpMap.find(strFile);
	if (iter != m_filesFpMap.end())
	{
		return iter->second;
	}

	return NULL;
}

// 设置进程日志目录
void CGameLogManage::SetLogPath(const std::string& path)
{
	m_logPath = path;
}

// 获取进程日志目录
std::string CGameLogManage::GetLogPath()
{
	return m_logPath;
}

std::mutex& CGameLogManage::GetMutex()
{ 
	return m_mutex; 
}

// 日志缓存
std::multimap<FILE*, std::string>& CGameLogManage::GetLogMap()
{
	return m_logMap;
}

// 日志打印
void CGameLogManage::Fflush()
{
	std::multimap<FILE*, std::string> logMap;

	m_mutex.lock();
	std::swap(logMap, m_logMap);
	m_mutex.unlock();

	for (std::multimap<FILE*, std::string>::const_iterator it = logMap.begin(); it != logMap.end(); ++it)
	{
		if (it->first)
		{
			fputs(it->second.c_str(), it->first);
			fflush(it->first);
		}
	}
}

// 日志处理线程
void CGameLogManage::HandlerLogThread()
{
	while (true)
	{
		// 日志五秒钟打印一次
		std::this_thread::sleep_for(std::chrono::seconds(8));
		if (!m_logMap.empty())
		{
			Fflush();
		}
	}
	COUT_LOG(LOG_CINFO, "log thread end...");
}