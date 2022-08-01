#pragma once

const int MAX_LOG_BUF_SIZE = 1024;		// log��������С
const int MAX_FILE_NAME_SIZE = 256;		// ����ļ�������
const int MAX_FUNC_NAME_SIZE = 128;		// �����������
const int MAX_LOG_FILE_SIZE = 1024 * 1024 * 30;  //�����ļ������ĳ��ȣ��������Ȼ������µ��ļ�

//���̴߳�ӡ��־��ʱ��Ҫ�ٿ�����
// ��Ϸlog����
class CGameLogManage
{
private:
	CGameLogManage();
	virtual ~CGameLogManage();

public:
	static CGameLogManage& Instance();

public:
	void Release();
	// ���÷���������
	void SetLogFileType(ServiceType serviceType);
	// ��ȡ��Ӧ�̵߳�errorlog
	std::string GetErrorLog();
	// ����ָ���ļ���fp
	bool AddLogFileFp(std::string strFile, FILE* fp);
	// ��ȡָ���ļ���fp
	FILE* GetLogFileFp(std::string&& strFile);
	// ���ý�����־Ŀ¼
	void SetLogPath(const std::string& path);
	// ��ȡ������־Ŀ¼
	std::string GetLogPath();
	// �����
	std::mutex& GetMutex();
	// ��־����
	std::list<std::pair<FILE*, std::string>>& GetLogMap();
	// ��־��ӡ
	void Fflush(char* logBuf);
	void Init(bool& run);

private:
	// ��־�����߳�
	void HandlerLogThread(bool& run);
	int GetLogHour(const std::string& str);

private:
	std::mutex m_mutex;
	// ��־�ļ�map
	std::map<ServiceType, std::string> m_LogFilesMap;
	// �ļ�������map
	std::unordered_map<std::string, FILE*> m_filesFpMap;
	// ������־Ŀ¼
	std::string m_logPath;
	//����������
	ServiceType m_serviceType;
	// ��־����
	std::list<std::pair<FILE*, std::string>> m_logMap;
	// ��־�߳�
	std::vector<std::thread*> m_threadVec;
};

//// ��־��
class CLog
{
public:
	CLog() {}
	virtual ~CLog() {}

public:
	static void Write(const char* pLogFile, int level, const char* pFile, int line, const char* pFuncName, const char* pFormat, ...);
};
