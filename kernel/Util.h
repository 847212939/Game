#pragma once
#include <iomanip>
#include <chrono>
#include <random>
#include <sstream>

namespace DRMU
{
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
		// ����UUIDȫ�ֺ���
		static std::string CreateUuid();

	};
}

template<class T>
DRMU::COstringstream& DRMU::COstringstream::operator << (T t)
{
	m_os << t << "\n";
	return *this;
}

template<class T>
DRMU::CIstringstream& DRMU::CIstringstream::operator >> (T& t)
{
	m_is >> t;
	++m_cnt;
	return *this;
}
