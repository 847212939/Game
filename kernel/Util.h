#pragma once

class Util
{
public:
	static Util* Instance();

private:
	Util();
	virtual~Util();

protected:
	Util(const Util& my);
	Util& operator=(const Util& my);

public:
	// 获取随机数
	static unsigned int GetRandNum();
	static int GetRandRange(int iMin, int iMax);

public:
	static void Exit(bool& run);

public:
	// 对称加密
	static char* Encrypt(char* content, size_t length);
	static char* Decrypt(char* content, size_t length);

public:
	// 配置转化成秒
	static uint64_t GetSysSecond();
	static long long GetSysMilliseconds();
	static uint64_t GetCfgSecond(const VectorInt& vec);
	static uint64_t GetCfgSecondEnd(const VectorInt& vec);

public:
	// 获取开服时间
	static int GetServiceDays();
	static uint64_t GetOpenServerTime();
	static bool InitTime();

public:
	// 生产怪
	static Animal* CreatAnimal(AnimalType type, int mid = 0);

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
	IDGen& GetIDGen();
	uint64_t CreateUserId();
	TCPClient* GetTCPClient();

private:
	static std::random_device	m_rd;
	static std::mt19937			m_mt;
	IDGen*						m_IDGen;
	TCPClient*					m_TCPClient;
	static time_t				m_OpenServerTimeSecond;
};
