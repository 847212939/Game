#include "../stdafx.h"

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
		// 最后一次拆分
		if (count > 0 && ++cnt >= count)
		{
			m_Splits.emplace_back(str.substr(i, size - i));
			break;
		}
		pos = str.find(separator, i);
		if (pos == std::string::npos)
		{
			// 防止最后没有结尾分隔符保留
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

Netmsg& Netmsg::operator >> (std::string& t)
{
	if (m_cnt < m_Splits.size())
	{
		t = m_Splits[m_cnt++];
	}
	return *this;
}