#pragma once

// �ַ���������.
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

// �ַ��������.
class CIstringstream
{
public:
	virtual ~CIstringstream();
	CIstringstream(std::string str);

public:
	template<class T>
	CIstringstream& operator >> (T& t);

	// ����ַ���
	CIstringstream& operator >> (std::string& outStr);
	CIstringstream& operator >> (char* pBuf);
	CIstringstream& operator >> (unsigned char* pBuf);

private:
	// ��¼������ε�����
	size_t				m_cnt;
	std::istringstream	m_is;
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

class Util
{
public:
	static Util* Instance();

private:
	Util(){}
	virtual~Util(){}

public:
	// ��ȡ�����
	static unsigned int GetRandNum();
	static int GetRandRange(int iMin, int iMax);

public:
	// ΨһID
	IDGen& GetIDGen();
	uint64_t CreateUserId();

public:
	// �����˳�
	static void Exit(bool& run, TCPClient& clien);

private:
	static std::random_device	m_rd;
	static std::mt19937			m_mt;
	IDGen						m_IDGen;
};
