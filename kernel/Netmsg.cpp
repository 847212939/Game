#include "../Game/stdafx.h"

Netmsg::operator std::string()
{
	return m_os.str();
}

std::string Netmsg::str()
{
	return m_os.str();
}

Netmsg::Netmsg() : m_cnt(0)
{

}

Netmsg::~Netmsg()
{

}

Netmsg::Netmsg(std::string str) : m_is(str), m_cnt(0)
{
}

Netmsg& Netmsg::operator >> (std::string& outStr)
{
	std::string str = m_is.str();
	size_t index1 = 0, index2 = 0, cnt = 0, len = 0, block = -1;
	while (true)
	{
		index2 = str.find("\n", index1);
		if (index2 == std::string::npos)
		{
			break;
		}
		if (cnt >= m_cnt)
		{
			outStr = str.substr(index1, index2 - index1);
			break;
		}

		++cnt;
		index1 = index2 + 1;
	}
	// 处理空格
	for (char& c : outStr)
	{
		++len;
		if (c != ' ' && len != block)
		{
			if (len - block != 1)
			{
				std::string dump;
				m_is >> dump;
			}
			block = len;
		}
	}

	++m_cnt;
	return *this;
}

Netmsg& Netmsg::operator >> (char* pBuf)
{
	std::string outStr;
	std::string str = m_is.str();
	size_t index1 = 0, index2 = 0, cnt = 0, len = 0, block = -1;
	while (true)
	{
		index2 = str.find("\n", index1);
		if (index2 == std::string::npos)
		{
			break;
		}
		if (cnt >= m_cnt)
		{
			outStr = str.substr(index1, index2 - index1);
			memcpy(pBuf, outStr.c_str(), outStr.size());
			break;
		}

		++cnt;
		index1 = index2 + 1;
	}
	// 处理空格
	for (char& c : outStr)
	{
		++len;
		if (c != ' ' && len != block)
		{
			if (len - block != 1)
			{
				std::string dump;
				m_is >> dump;
			}
			block = len;
		}
	}

	++m_cnt;
	return *this;
}

Netmsg& Netmsg::operator >> (unsigned char* pBuf)
{
	std::string outStr;
	std::string str = m_is.str();
	size_t index1 = 0, index2 = 0, cnt = 0, len = 0, block = -1;
	while (true)
	{
		index2 = str.find("\n", index1);
		if (index2 == std::string::npos)
		{
			break;
		}
		if (cnt >= m_cnt)
		{
			outStr = str.substr(index1, index2 - index1);
			memcpy(pBuf, outStr.c_str(), outStr.size());
			break;
		}

		++cnt;
		index1 = index2 + 1;
	}
	// 处理空格
	for (char& c : outStr)
	{
		++len;
		if (c != ' ' && len != block)
		{
			if (len - block != 1)
			{
				std::string dump;
				m_is >> dump;
			}
			block = len;
		}
	}

	++m_cnt;
	return *this;
}

void Netmsg::split(std::string& str, std::string separator)
{
	std::vector<std::string> result;

	size_t pos = 0;
	str += separator;
	size_t size = str.size();

	for (size_t i = 0; i < size; i++)
	{
		pos = str.find(separator, i);
		if (pos == std::string::npos)
		{
			break;
		}
		result.push_back(str.substr(i, pos - i));
		i = pos + separator.size() - 1;
	}
}