#pragma once
#include <list>

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
	std::ostringstream		m_os;
	std::istringstream		m_is;
	std::list<std::string>	m_SplitsList;
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