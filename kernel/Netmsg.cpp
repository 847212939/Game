#include "../Game/stdafx.h"

Netmsg::Netmsg() : m_cnt(0)
{

}

Netmsg::~Netmsg()
{

}

Netmsg::Netmsg(std::string str, int count) : m_cnt(0)
{
	split(str, "\n", count);
}

Netmsg::operator std::string()
{
	return m_os.str();
}

std::string Netmsg::str()
{
	return m_os.str();
}

//字符串分割函数
void Netmsg::split(std::string& str, std::string separator, size_t count)
{
	size_t pos = 0, cnt = 0;
	size_t size = str.size();

	for (size_t i = 0; i < size; i++)
	{
		// 最后一次拆分
		if (count > 0 && ++cnt >= count)
		{
			m_SplitsVec.push_back(str.substr(i, size - i));
			break;
		}
		pos = str.find(separator, i);
		if (pos == std::string::npos)
		{
			break;
		}
		m_SplitsVec.push_back(str.substr(i, pos - i));
		i = pos + separator.size() - 1;
	}
}
