#pragma once
#include <map>
#include <string>
#include <mutex>
#include <unordered_map>

namespace DRMU
{
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
		static CGameLogManage* Instance();

	public:
		void Release();
		// ���÷���������
		void SetLogFileType(int threadType);
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
		//�����
		std::mutex& GetMutex();

	private:
		std::mutex m_mutex;
		// ��־�ļ�map
		std::map<int, std::string> m_LogFilesMap;
		// �ļ�������map
		std::unordered_map<std::string, FILE*> m_filesFpMap;
		// ������־Ŀ¼
		std::string m_logPath;
		//����������
		int m_serviceType;
	};

	//// ��־��
	class CLog
	{
	public:
		CLog() {}
		virtual ~CLog() {}

	public:
		static void Write(const char* pLogFile, int level, const char* pFile, int line, const char* pFuncName, const char* pFormat, ...);
		static void Write(const char* pLogFile, const char* pFuncName, const char* pFormat, ...);
		// ��buf�е�����д��logFile
		static void Write(const char* pLogFile, const char* buf);
		// ��buf�е�����д��logFile���ļ������û��Լ�����
		static void Write(const char* pLogFile, const char* pFile, int line, const char* pFuncName, const char* pBuf, ...);
	};
}

