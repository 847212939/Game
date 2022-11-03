#include "../stdafx.h"

Netmsg::Netmsg()
{

}
Netmsg::~Netmsg()
{

}
Netmsg::Netmsg(char* data, int len, int count)
{
	std::string separator = "\n";
	std::string str(data, len);

	size_t pos = 0, cnt = 0;
	size_t size = str.size();
	size_t separatorSize = separator.size();

	for (size_t i = 0; i < size; i++)
	{
		// 最后一次拆分
		if (count > 0 && ++cnt >= count)
		{
			m_SplitsList.push_back(str.substr(i, size - i));
			break;
		}
		pos = str.find(separator, i);
		if (pos == std::string::npos)
		{
			// 防止最后没有结尾分隔符保留
			if (i < size)
			{
				m_SplitsList.push_back(str.substr(i, size - i));
			}
			break;
		}
		m_SplitsList.push_back(str.substr(i, pos - i));
		i = pos + separatorSize - 1;
	}
}

size_t Netmsg::size()
{
	return m_SplitsList.size();
}

bool Netmsg::empty()
{
	return m_SplitsList.empty();
}

Netmsg::operator std::string()
{
	return m_os.str();
}
std::string Netmsg::str()
{
	return m_os.str();
}

Netmsg& Netmsg::operator >> (std::string& t)
{
	if (!m_SplitsList.empty())
	{
		t = m_SplitsList.front();
		m_SplitsList.pop_front();
	}
	return *this;
}