#pragma once

class Netmsg
{
public:
	Netmsg();
	virtual ~Netmsg();
	Netmsg(std::string str, int count = 0);

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

	void split(std::string& str, std::string separator, size_t count = 0);

private:
	std::ostringstream	m_os;
	size_t				m_cnt;

	std::vector<std::string> m_SplitsVec;
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
	if (m_cnt < m_SplitsVec.size())
	{
		std::istringstream is(m_SplitsVec[m_cnt]);
		is >> t;
		++m_cnt;
	}
	return *this;
}