#pragma once

class IDGen;
class Util
{
public:
	static Util* Instance();

public:
	static void InitCxxnet();

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
	static long long GetSysMilliseconds();

public:
	// �ԳƼ���
	static uint8_t* Encrypt(uint8_t* content, int length);
	static uint8_t* Decrypt(uint8_t* content, int length);

public:
	// ����ת������
	static uint64_t GetSysSecond();
	static uint64_t GetCfgSecond(const IntVector& vec);
	static uint64_t GetCfgSecondEnd(const IntVector& vec);

public:
	// ��ȡ����ʱ��
	static int GetServiceDays();
	static uint64_t GetOpenServerTime();
	static const struct tm& GetOpenServerTimeTM();
	static bool InitTime();

public:
	// ΨһID
	IDGen& GetIDGen();
	uint64_t CreateUserId();
	TCPClient* GetTCPClient();

private:
	static std::random_device	m_rd;
	static std::mt19937			m_mt;
	static struct tm			m_tm;
	static time_t				m_OpenServerTimeSecond;
	static int					m_day;
	IDGen*						m_IDGen;
	TCPClient*					m_TCPClient;
};

class Cos
{
public:
	Cos();
	virtual ~Cos();

protected:
	Cos(const Cos& my);
	Cos& operator=(const Cos& my);

public:
	template<class T>
	Cos& operator << (T t);

	operator std::string();
	std::string str();

private:
	std::ostringstream m_os;
};

class Cis
{
public:
	virtual ~Cis();
	Cis(std::string str);

protected:
	Cis(const Cis& my);
	Cis& operator=(const Cis& my);

public:
	template<class T>
	Cis& operator >> (T& t);

	// ����ַ���
	Cis& operator >> (std::string& outStr);
	Cis& operator >> (char* pBuf);
	Cis& operator >> (unsigned char* pBuf);

private:
	// ��¼������ε�����
	size_t				m_cnt;
	std::istringstream	m_is;
};

template<class T>
Cos& Cos::operator << (T t)
{
	m_os << t << "\n";
	return *this;
}

template<class T>
Cis& Cis::operator >> (T& t)
{
	m_is >> t;
	++m_cnt;
	return *this;
}

class IDGen
{
public:
	IDGen();
	virtual ~IDGen(){}

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
	uint64_t   m_addID;
};