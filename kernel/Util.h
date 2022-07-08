#pragma once

// 字符串输入流.
class COstringstream
{
public:
	COstringstream();
	virtual ~COstringstream();

public:
	template<class T>
	COstringstream& operator << (T t);

	operator std::string();
	std::string str();

private:
	std::ostringstream m_os;
};

// 字符串输出流.
class CIstringstream
{
public:
	virtual ~CIstringstream();
	CIstringstream(std::string str);

public:
	template<class T>
	CIstringstream& operator >> (T& t);

	// 输出字符串
	CIstringstream& operator >> (std::string& outStr);
	CIstringstream& operator >> (char* pBuf);
	CIstringstream& operator >> (unsigned char* pBuf);

private:
	// 记录输出几次的数量
	size_t				m_cnt;
	std::istringstream	m_is;
};

class IDGen
{
public:
	IDGen() : 
		m_serverType(), 
		m_serverID(), 
		m_lastSecond(0), 
		m_addID(0)
	{ }
	virtual ~IDGen(){}

public:
	void Init(int serverType, int serverID)
	{
		m_serverType = serverType;
		m_serverID = serverID;
	}
	uint64_t GenerateUID()
	{
		time_t curSecond = time(NULL);
		if (curSecond != m_lastSecond)
		{
			m_lastSecond = curSecond;
			m_addID = 0;
		}

		return ((uint64_t)m_serverType << 59) + ((uint64_t)m_serverID << 52) + ((uint64_t)curSecond << 20) + ((uint64_t)m_addID++);
	}

private:
	int         m_serverType;
	int	        m_serverID;
	time_t      m_lastSecond;
	uint64_t   m_addID;
};

class Util
{
public:
	static Util* Instance()
	{
		static Util g_mgr;
		return &g_mgr;
	}

private:
	Util(){}
	~Util(){}

public:
	static unsigned int GetRandNum();
	static int GetRandRange(int iMin, int iMax);

	IDGen& GetIDGen()
	{
		return m_IDGen;
	}
	
	uint64_t CreateUserId()
	{
		return m_IDGen.GenerateUID();
	}

private:
	static std::random_device	m_rd;
	static std::mt19937			m_mt;
	IDGen						m_IDGen;
};

template<class T>
COstringstream& COstringstream::operator << (T t)
{
	m_os << t << "\n";
	return *this;
}

template<class T>
CIstringstream& CIstringstream::operator >> (T& t)
{
	m_is >> t;
	++m_cnt;
	return *this;
}
