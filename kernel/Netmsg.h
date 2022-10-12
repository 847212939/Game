#pragma once

class Netmsg
{
public:
	/*
	* @brief 消息拆分构造函数
	* @param str 原始字符串
	* @param count 拆分为几个子串
	*/
	Netmsg(std::string str, int count = 0);
	/*
	* @brief 消息拆分构造函数
	*/
	Netmsg();
	virtual ~Netmsg();

protected:
	/*
	* @brief 禁用拷贝构造函数
	*/
	Netmsg(const Netmsg& my);
	Netmsg& operator=(const Netmsg& my);

public:
	template<class T>
	Netmsg& operator << (T t);
	template<class T>
	Netmsg& operator >> (T& t);

public:
	std::string str();
	operator std::string();

public:
	/*
	* @brief 字符串拆分函数
	* @param str 原始字符串
	* @param separator 分隔符
	* @param count 拆分为几个子串
	* @return ListString
	*/
	static ListString StringSplit(std::string& str,
		std::string separator, size_t count = 0);

private:
	std::ostringstream	m_os;
	std::istringstream	m_is;
	ListString			m_SplitsList;
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
	if (!m_SplitsList.empty())
	{
		m_is.clear();
		m_is.str(m_SplitsList.front());
		m_is >> t;
		m_SplitsList.pop_front();
	}
	return *this;
}