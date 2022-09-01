#include "pch.h"

std::string Util::m_key = "";

Util* Util::Instance()
{
	static Util g_mgr;
	return &g_mgr;
}

Util::Util() : m_TCPClient(nullptr)
{
}

Util::~Util()
{
}

bool Util::InitCxxnet(NetworkCallBackFunc netFunc, TimerCallBackFunc timerFunc, CloseCallBackFunc closeFunc)
{
	if (!m_TCPClient->Init(netFunc, timerFunc, closeFunc))
	{
		return false;
	}
	if (!m_TCPClient->GetRuninged())
	{
		return false;
	}

	return true;
}

TCPClient* Util::GetTCPClient()
{
	if (!m_TCPClient)
	{
		m_TCPClient = new TCPClient;
	}
	return m_TCPClient;
}

std::string& Util::GetEncryptKey()
{
	return m_key;
}

void Util::SetEncryptKey(std::string key)
{
	m_key = key;
}

long long Util::GetSysMilliseconds()
{
	auto time_now = std::chrono::system_clock::now();
	auto duration_in_ms = std::chrono::duration_cast<std::chrono::milliseconds>(time_now.time_since_epoch());
	return duration_in_ms.count();
}

char* Util::Encrypt(char* content, size_t length)
{
	for (size_t i = 0; i < length; i++)
	{
		content[i] ^= m_key[i % (m_key.size() - 1)];
	}
	return content;
}

char* Util::Decrypt(char* content, size_t length)
{
	for (size_t i = 0; i < length; i++)
	{
		content[i] ^= m_key[i % (m_key.size() - 1)];
	}
	return content;
}


Cos::Cos()
{
}

Cos::~Cos()
{

}

Cos::operator std::string()
{
	return m_os.str();
}

std::string Cos::str()
{
	return m_os.str();
}

Cis::~Cis()
{

}

Cis::Cis(std::string str) : m_is(str), m_cnt(0)
{
}

Cis& Cis::operator >> (std::string& outStr)
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

Cis& Cis::operator >> (char* pBuf)
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

Cis& Cis::operator >> (unsigned char* pBuf)
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
