#define _CRT_SECURE_NO_WARNINGS
#include "../Game/stdafx.h"

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
	sprintf(buf + strlen(buf), "%s ", levelName);

	// 参数
	va_list args;
	va_start(args, pBuf);

	vsprintf(buf + strlen(buf), pBuf, args);
	va_end(args);

	// 输出到控制台
	if (level == LOG_CINFO)
	{
		std::cout << buf << std::endl;
	}
	else if (level == LOG_CERROR)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		std::cout << buf << "{func=" << pFuncName << " line=" << line << "}" << std::endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	}

	sprintf(buf + strlen(buf), "{%s %s %d}\n", pFile, pFuncName, line);

	std::string strPath = LogManager()->GetLogPath() + pLogfile;

	FILE* fp = LogManager()->GetLogFileFp(std::move(pLogfile));
	if (!fp)
	{
		fp = fopen(strPath.c_str(), "a+");
		if (!fp)
		{
			return;
		}
		LogManager()->AddLogFileFp(pLogfile, fp);
	}

	std::lock_guard<std::mutex> guard(LogManager()->GetMutex());

	fputs(buf, fp);
	fflush(fp);

}

void CLog::Write(const char* pLogFile, const char* pFuncName, const char* pFormat, ...)
{
	if (!pLogFile || !pFuncName || !pFormat)
	{
		return;
	}

	char buf[MAX_LOG_BUF_SIZE] = "";

	// 时间
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);

	sprintf(buf, "[%04d-%02d-%02d %02d:%02d:%02d]", sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	// 线程ID和level
	const char* levelName = levelNames[LOG_INFO];
	sprintf(buf + strlen(buf), "%s ", levelName);

	// 参数
	va_list args;
	va_start(args, pFormat);

	vsprintf(buf + strlen(buf), pFormat, args);
	va_end(args);

	sprintf(buf + strlen(buf), " %s\n", pFuncName);

	std::string strPath = LogManager()->GetLogPath() + pLogFile;

	FILE* fp = LogManager()->GetLogFileFp(std::move(pLogFile));
	if (!fp)
	{
		fp = fopen(strPath.c_str(), "a+");
		if (!fp)
		{
			return;
		}
		LogManager()->AddLogFileFp(pLogFile, fp);
	}

	std::lock_guard<std::mutex> guard(LogManager()->GetMutex());

	fputs(buf, fp);
	fflush(fp);
}

void CLog::Write(const char* pLogFile, const char* buf)
{
	if (!pLogFile || !buf)
	{
		return;
	}

	std::string strPath = LogManager()->GetLogPath() + pLogFile;

	FILE* fp = LogManager()->GetLogFileFp(std::move(pLogFile));
	if (!fp)
	{
		fp = fopen(strPath.c_str(), "a+");
		if (!fp)
		{
			return;
		}
		LogManager()->AddLogFileFp(pLogFile, fp);
	}

	std::lock_guard<std::mutex> guard(LogManager()->GetMutex());

	fputs(buf, fp);
	fflush(fp);
}

void CLog::Write(const char* pLogFile, const char* pFile, int line, const char* pFuncName, const char* pBuf, ...)
{
	if (!pLogFile || !pFile || !pFuncName || !pBuf)
	{
		return;
	}

	char buf[MAX_LOG_BUF_SIZE] = "";

	// 时间
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);

	sprintf(buf, "[%04d-%02d-%02d %02d:%02d:%02d] ", sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

	// 参数
	va_list args;
	va_start(args, pBuf);

	vsprintf(buf + strlen(buf), pBuf, args);
	va_end(args);

	sprintf(buf + strlen(buf), "\n");
	//sprintf(buf + strlen(buf), "{%s %s %d}\n", pFile, pFuncName, line);

	std::string strPath = LogManager()->GetLogPath();
	std::string strFile = pLogFile;
	strFile += ".log";
	strPath += strFile;
	FILE* fp = LogManager()->GetLogFileFp(std::move(strFile));
	if (!fp)
	{
		fp = fopen(strPath.c_str(), "a+");
		if (!fp)
		{
			return;
		}
		LogManager()->AddLogFileFp(strFile.c_str(), fp);
	}

	std::lock_guard<std::mutex> guard(LogManager()->GetMutex());

	fputs(buf, fp);
	fflush(fp);
}

CGameLogManage::CGameLogManage()
{
	char filePath[512] = "";
	GetCurrentDirectoryA(512, filePath);
	std::string strPth = filePath;
	strPth += "\\log\\";

	m_logPath = strPth;
	m_serviceType = 0;
}

CGameLogManage::~CGameLogManage()
{
	//Release();
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

void CGameLogManage::SetLogFileType(int serviceType)
{
	if (serviceType <= SERVICE_TYPE_BEGIN || serviceType >= SERVICE_TYPE_END)
	{
		return;
	}
	std::string nameStr;
	if (serviceType == SERVICE_TYPE_LOGIC)
	{
		nameStr += "LogicServer";
	}
	else if (serviceType == SERVICE_TYPE_DB)
	{
		nameStr += "DBServer";
	}
	else if (serviceType == SERVICE_TYPE_GATA)
	{
		nameStr += "GataServer";
	}
	else if (serviceType == SERVICE_TYPE_GAMECENTER)
	{
		nameStr += "GameCenterServer";
	}

	m_serviceType = serviceType;
	m_LogFilesMap.emplace(serviceType, nameStr);
}

std::string CGameLogManage::GetErrorLog()
{
	if (m_serviceType <= SERVICE_TYPE_BEGIN || m_serviceType >= SERVICE_TYPE_END)
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