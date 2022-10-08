﻿#include "../Game/stdafx.h"

// 方法类
std::mt19937		Util::m_mt(m_rd());
std::random_device	Util::m_rd;
time_t				Util::m_OpenServerTimeSecond = 0;

Util* Util::Instance()
{
	static Util g_mgr;
	return &g_mgr;
}

Util::Util() : 
	m_IDGen(new IDGen), 
	m_TCPClient(new TCPClient)
{
}

Util::~Util() 
{

}

// 获取随机数
unsigned int Util::GetRandNum()
{
	return m_mt();
}

// 获取[A,B)随机数,min<= 随机数 < iMax
int Util::GetRandRange(int iMin, int iMax)
{
	if (iMin >= iMax)
	{
		return GetRandNum();
	}

	return iMin + (int)(GetRandNum() % (iMax - iMin));
}

IDGen& Util::GetIDGen()
{
	return *m_IDGen;
}

uint64_t Util::CreateUserId()
{
	return m_IDGen->GenerateUID();
}

IDGen::IDGen() :
	m_serverType(0),
	m_serverID(0),
	m_lastSecond(0),
	m_addID(0)
{ 

}

void IDGen::Init(int serverType, int serverID)
{
	m_serverType = serverType;
	m_serverID = serverID;
}

uint64_t IDGen::GenerateUID()
{
	time_t curSecond = time(nullptr);
	if (curSecond != m_lastSecond)
	{
		m_lastSecond = curSecond;
		m_addID = 0;
	}

	return ((uint64_t)m_serverType << 59) + 
		((uint64_t)m_serverID << 52) + 
		((uint64_t)curSecond << 20) + 
		((uint64_t)m_addID++);
}

void Util::Exit(bool& run)
{
	std::string str;

	while (run)
	{
		std::cin >> str;

		if (str == BaseCfgMgr.GetExit())
		{
			run = false;
			break;
		}
	}

	DTCPC->Stop();
	DTCPC->NotifyAll();
}

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

long long Util::GetSysMilliseconds()
{
	auto time_now = std::chrono::system_clock::now();
	auto duration_in_ms = std::chrono::duration_cast<std::chrono::milliseconds>(time_now.time_since_epoch());
	return duration_in_ms.count();
}

char* Util::Encrypt(char* content, size_t length)
{
	std::string sKey = BaseCfgMgr.GetKey();
	for (size_t i = 0; i < length; i++)
	{
		content[i] ^= sKey[i % (sKey.size() - 1)];
	}
	return content;
}

char* Util::Decrypt(char* content, size_t length)
{
	std::string sKey = BaseCfgMgr.GetKey();
	for (size_t i = 0; i < length; i++)
	{
		content[i] ^= sKey[i % (sKey.size() - 1)];
	}
	return content;
}

TCPClient* Util::GetTCPClient()
{
	return m_TCPClient;
}

uint64_t Util::GetCfgSecond(const IntVector& vec)
{
	if (vec.size() < 3)
	{
		return 0;
	}

	return ((uint64_t)vec[0]) * 60 * 60 + ((uint64_t)vec[1]) * 60 + (uint64_t)vec[2];
}

uint64_t Util::GetCfgSecondEnd(const IntVector& vec)
{
	if (vec.size() < 6)
	{
		return 0;
	}

	return ((uint64_t)vec[3]) * 60 * 60 + ((uint64_t)vec[4]) * 60 + (uint64_t)vec[5];
}

uint64_t Util::GetSysSecond()
{
	time_t tick = ::time(nullptr);
	struct tm tm;
	tm = *localtime(&tick);

	return ((uint64_t)tm.tm_hour) * 60 * 60 + ((uint64_t)tm.tm_min) * 60 + (uint64_t)tm.tm_sec;
}

uint64_t Util::GetOpenServerTime()
{
	return m_OpenServerTimeSecond;
}

int Util::GetServiceDays()
{
	// 开服天数
	return (int)(
		(::time(nullptr) 
		- m_OpenServerTimeSecond) 
		/ (60 * 60 * 24)) 
		+ 1;
}

bool Util::InitTime()
{
	struct tm tm1;
	std::string openServerTime = BaseCfgMgr.GetOpenServerTime();
	if (sscanf(openServerTime.c_str(), "%4d-%2d-%2d %2d:%2d:%2d", 
	&tm1.tm_year, &tm1.tm_mon, &tm1.tm_mday, &tm1.tm_hour, 
	&tm1.tm_min, &tm1.tm_sec) <= 0 || openServerTime.empty())
	{
		return false;
	}

	struct tm tm;
	tm = tm1;
	tm.tm_year = tm1.tm_year - 1900;
	tm.tm_mon = tm1.tm_mon - 1;
	tm.tm_mday = tm1.tm_mday;

	m_OpenServerTimeSecond = mktime(&tm);

	return true;
}

Animal* Util::CreatAnimal(AnimalType type, int mid)
{
	switch (type)
	{
	case AnimalType::at_player:
	{
		return nullptr;
	}
	case AnimalType::at_monster:
	{
		Animal* animal = new MonsterClient;
		dynamic_cast<MonsterClient*>(animal)->Init(mid);
		return animal;
	}
	default:
		return nullptr;
	}
	return nullptr;
}