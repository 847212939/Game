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
	// ��ȡ�����
	static unsigned int GetRandNum();
	static int GetRandRange(int iMin, int iMax);

public:
	static void Exit(bool& run);

public:
	// �ԳƼ���
	static char* Encrypt(char* content, size_t length);
	static char* Decrypt(char* content, size_t length);

public:
	// ����ת������
	static uint64_t GetSysSecond();
	static long long GetSysMilliseconds();
	static uint64_t GetCfgSecond(const IntVector& vec);
	static uint64_t GetCfgSecondEnd(const IntVector& vec);

public:
	// ��ȡ����ʱ��
	static int GetServiceDays();
	static uint64_t GetOpenServerTime();
	static bool InitTime();

public:
	// ������
	static Animal* CreatAnimal(AnimalType type, int mid = 0);

public:
	// ΨһID
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
	// ��¼������ε�����
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
