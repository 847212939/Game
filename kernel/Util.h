#pragma once
#include <iomanip>
#include <chrono>
#include <random>
#include <sstream>

namespace DRMU
{
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

	// 方法类
	class Util
	{
	public:
		// 生成UUID全局函数
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
