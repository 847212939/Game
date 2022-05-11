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

// ������
class Util
{
public:
	Util();
	~Util();
public:
	// ����UUIDȫ�ֺ���
	static std::string CreateUuid();
	// ��ȡһ�������
	static unsigned int GetRandNum();
	// ��ȡ[A,B)�����,min<= ����� < iMax
	static int GetRandRange(int iMin, int iMax);
private:
	static std::random_device	m_rd;
	static std::mt19937			m_mt;
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
