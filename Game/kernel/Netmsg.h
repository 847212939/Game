#pragma once

class Netmsg
{
public:
	/*
	* @brief ��Ϣ��ֹ��캯��
	* @param str ԭʼ�ַ���
	* @param count ���Ϊ�����Ӵ�
	*/
	Netmsg(std::string str, int count = 0);
	/*
	* @brief ��Ϣ��ֹ��캯��
	*/
	Netmsg();
	virtual ~Netmsg();

protected:
	/*
	* @brief ���ÿ������캯��
	*/
	Netmsg(const Netmsg& msg);
	Netmsg& operator=(const Netmsg& msg);

public:
	template<class T>
	Netmsg& operator << (T t);
	template<class T>
	Netmsg& operator >> (T& t);

public:
	std::string str();
	operator std::string();
	size_t size();
	bool empty();

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