#pragma once

class Netmsg
{
public:
	Netmsg(int client = 0);
	virtual ~Netmsg();
	/*
	* @brief 消息拆分构造函数
	* @param str 原始字符串
	* @param client 0是客户端消息 1是数据库消息
	* @param count 拆分为几个子串
	*/
	Netmsg(std::string str, int client = 0, int count = 0);

protected:
	// 禁用拷贝构造函数
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
	std::istringstream	m_is;

	ListString			m_SplitsList;
	int					m_client;
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

#ifdef __DEBUG__
		if (m_client == 0)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
			printf("客户端传来消息: %s\n", m_is.str().c_str());
		}
		else if (m_client == 1)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);
			printf("数据库传来消息: %s\n", m_is.str().c_str());
		}
		if (m_SplitsList.empty())
		{
			printf("---end---\n");
		}
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#endif // __DEBUG__
	}
	return *this;
}