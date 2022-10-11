#include "../Game/stdafx.h"

Netmsg::Netmsg(int client) : m_client(client)
{

}

Netmsg::~Netmsg()
{

}

Netmsg::Netmsg(std::string str, int client, int count) : m_client(client)
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

//�ַ����ָ��
void Netmsg::split(std::string& str, std::string separator, size_t count)
{
	size_t pos = 0, cnt = 0;
	size_t size = str.size();

	for (size_t i = 0; i < size; i++)
	{
		// ���һ�β��
		if (count > 0 && ++cnt >= count)
		{
			m_SplitsList.push_back(str.substr(i, size - i));
			break;
		}
		pos = str.find(separator, i);
		if (pos == std::string::npos)
		{
			break;
		}
		m_SplitsList.push_back(str.substr(i, pos - i));
		i = pos + separator.size() - 1;
	}
}

