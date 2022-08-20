﻿#include "pch.h"

void Util::InitCxxnet(pfCallBackEvent func)
{
	if (!LuaMgr->InitCfgMgr())
	{
		std::cout << "InitCxxnet exit" << std::endl;
		return;
	}
	if (!m_TCPClient->Init(func))
	{
		std::cout << "InitCxxnet exit" << std::endl;
		return;
	}

	return;
}

Util* Util::Instance()
{
	static Util g_mgr;
	return &g_mgr;
}

Util::Util() : m_TCPClient(new TCPClient)
{
}

Util::~Util() 
{

}

TCPClient* Util::GetTCPClient()
{
	return m_TCPClient;
}

long long Util::GetSysMilliseconds()
{
	auto time_now = std::chrono::system_clock::now();
	auto duration_in_ms = std::chrono::duration_cast<std::chrono::milliseconds>(time_now.time_since_epoch());
	return duration_in_ms.count();
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

uint8_t* Util::Encrypt(uint8_t* content, int length)
{
	std::string sKey = BaseCfgMgr.GetKey();

	for (size_t i = 0; i < length; i++)
	{
		content[i] ^= sKey[i % (sizeof(sKey) - 1)];
	}
	return content;
}

uint8_t* Util::Decrypt(uint8_t* content, int length)
{
	std::string sKey = BaseCfgMgr.GetKey();

	for (size_t i = 0; i < length; i++)
	{
		content[i] ^= sKey[i % (sizeof(sKey) - 1)];
	}
	return content;
}
