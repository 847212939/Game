#pragma once

class IDGen;
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
	static uint64_t GetCfgSecond(const IntVector& vec);
	static uint64_t GetCfgSecondEnd(const IntVector& vec);

public:
	// 获取开服时间
	static int GetServiceDays();
	static uint64_t GetOpenServerTime();
	static bool InitTime();

public:
	// 生产怪
	static Animal* CreatAnimal(AnimalType type, int mid = 0);

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

class IDGen
{
public:
	IDGen();
	virtual ~IDGen() {}

protected:
	IDGen(const IDGen& my);
	IDGen& operator=(const IDGen& my);

public:
	void Init(int serverType, int serverID);
	uint64_t GenerateUID();

private:
	int         m_serverType;
	int	        m_serverID;
	time_t      m_lastSecond;
	uint64_t	m_addID;
};

class Netmsg
{
public:
	Netmsg();
	virtual ~Netmsg();
	Netmsg(std::string str);

protected:
	Netmsg(const Netmsg& my);
	Netmsg& operator=(const Netmsg& my);

public:
	std::string str();
	operator std::string();

	template<class T>
	Netmsg& operator << (T t);

	template<class T>
	Netmsg& operator >> (T& t);
	Netmsg& operator >> (char* pBuf);
	Netmsg& operator >> (unsigned char* pBuf);
	Netmsg& operator >> (std::string& outStr);

private:
	// 记录输出几次的数量
	std::ostringstream	m_os;
	std::istringstream	m_is;
	size_t				m_cnt;
};

template<class T>
Netmsg& Netmsg::operator << (T t)
{
	m_os << t << "\n";
	return *this;
}

template<class T>
Netmsg& Netmsg::operator >> (T& t)
{
	m_is >> t;
	++m_cnt;
	return *this;
}
