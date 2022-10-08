#pragma once

class Netmsg
{
public:
	Netmsg();
	Netmsg(std::string str);
	virtual ~Netmsg();

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

	void split(std::string& str, std::string separator);

private:
	// 记录输出几次的数量
	std::ostringstream	m_os;
	std::istringstream	m_is;
	size_t				m_cnt;

protected:
	Netmsg(const Netmsg& my);
	Netmsg& operator=(const Netmsg& my);
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
