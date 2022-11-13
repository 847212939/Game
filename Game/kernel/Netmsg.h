#pragma once

class Netmsg
{
public:
	/*
	* @brief ��Ϣ��ֹ��캯��
	* @param data ԭʼ�ַ���
	* @param size �ַ�������
	* @param count ���Ϊ�����Ӵ�
	*/
	Netmsg(char* data, int len, int count = 0);
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
	Netmsg& operator >> (std::string& t);

public:
	std::string os_str()
	{
		return m_os.str();
	}
	size_t os_size()
	{
		return m_os.str().size();
	}
	size_t is_size()
	{
		return m_Splits.size();
	}
	bool is_empty()
	{
		return m_Splits.empty();
	}

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

Netmsg& Netmsg::operator >> (std::string& t)
{
	if (m_cnt < m_Splits.size())
	{
		t = m_Splits[m_cnt++];
	}
	return *this;
}

Netmsg::Netmsg() : m_cnt(0)
{

}
Netmsg::~Netmsg()
{

}
Netmsg::Netmsg(char* data, int len, int count) : m_cnt(0)
{
	std::string separator = "\n";
	std::string str(data, len);

	size_t pos = 0, cnt = 0;
	size_t size = str.size();
	size_t separatorSize = separator.size();

	for (size_t i = 0; i < size; i++)
	{
		// ���һ�β��
		if (count > 0 && ++cnt >= count)
		{
			m_Splits.emplace_back(str.substr(i, size - i));
			break;
		}
		pos = str.find(separator, i);
		if (pos == std::string::npos)
		{
			// ��ֹ���û�н�β�ָ�������
			if (i < size)
			{
				m_Splits.emplace_back(str.substr(i, size - i));
			}
			break;
		}
		m_Splits.emplace_back(str.substr(i, pos - i));
		i = pos + separatorSize - 1;
	}
}
