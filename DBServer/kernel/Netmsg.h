#pragma once

class Netmsg
{
public:
	/*
	* @brief 消息拆分构造函数
	* @param data 原始字符串
	* @param size 字符串长度
	* @param count 拆分为几个子串
	*/
	Netmsg(char* data, int len, int count = 0);
	/*
	* @brief 消息拆分构造函数
	*/
	Netmsg();
	virtual ~Netmsg();

protected:
	/*
	* @brief 禁用拷贝构造函数
	*/
	Netmsg(const Netmsg& msg);
	Netmsg& operator=(const Netmsg& msg);

public:
	template<class T>
	Netmsg& operator << (T t);
	template<class T>
	Netmsg& operator >> (T& t);
	Netmsg& operator >> (std::string& t);

public:
	std::string str();
	operator std::string();
	size_t size();
	bool empty();

private:
	std::ostringstream	m_os;
	std::istringstream	m_is;
	VectorString		m_Splits;
	int					m_cnt;
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
	if (m_cnt < m_Splits.size())
	{
		m_is.clear();
		m_is.str(m_Splits[m_cnt++]);
		m_is >> t;
	}
	return *this;
}