#pragma once

class IDGen;
class Util
{
public:
	static Util* Instance();

private:
	Util();
	virtual~Util();

public:
	// ��ȡ�����
	static unsigned int GetRandNum();
	static int GetRandRange(int iMin, int iMax);

	static void Exit(bool& run, TCPClient& clien);
	static long long GetSysMilliseconds();

	// �ԳƼ���
	static uint8_t* Encrypt(uint8_t* content, int length);
	static uint8_t* Decrypt(uint8_t* content, int length);

public:
	// ΨһID
	IDGen& GetIDGen();
	uint64_t CreateUserId();

private:
	static std::random_device	m_rd;
	static std::mt19937			m_mt;
	IDGen*						m_IDGen;
};

class Cos
{
public:
	Cos();
	virtual ~Cos();

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

public:
	void Init(int serverType, int serverID);
	uint64_t GenerateUID();

private:
	int         m_serverType;
	int	        m_serverID;
	time_t      m_lastSecond;
	uint64_t   m_addID;
};
