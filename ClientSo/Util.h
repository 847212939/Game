#pragma once

class Util
{
public:
	static Util* Instance();

private:
	Util();
	virtual~Util();

protected:
	// 禁用拷贝构造函数
	Util(const Util& my);
	Util& operator=(const Util& my);

public:
	// 获取随机数
	static unsigned int GetRandNum();
	// 获取[A,B)随机数,min<= 随机数 < iMax
	static int GetRandRange(int iMin, int iMax);

public:
	// 程序退出
	static void Exit(bool& run);
	// 加密
	static char* Encrypt(char* content, size_t length);
	// 解密
	static char* Decrypt(char* content, size_t length);
	static std::string& GetEncryptKey();
	static void SetEncryptKey(std::string key);

public:
	// 获取计算机秒数
	static uint64_t GetSysSecond();
	// 获取计算机毫秒数
	static long long GetSysMilliseconds();
	// 获取配置转换成秒数
	static uint64_t GetCfgSecond(const VectorInt& vec);
	// 获取配置转换成秒数
	static uint64_t GetCfgSecondEnd(const VectorInt& vec);

public:
	/*
	* @brief 字符串拆分函数
	* @param str 原始字符串
	* @param separator 分隔符
	* @param count 拆分为几个子串
	* @return ListString
	*/
	static ListString StringSplit(std::string& str, std::string separator, size_t count = 0);

public:
	// 唯一ID
	uint64_t CreateUserId();
	TCPClient* GetTCPClient();

private:
	static std::random_device	m_rd;
	static std::mt19937			m_mt;
	TCPClient*					m_TCPClient;
	static time_t				m_OpenServerTimeSecond;
	static std::string			m_key;
};
