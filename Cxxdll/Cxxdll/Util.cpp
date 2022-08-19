#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"

// 方法类
std::mt19937		Util::m_mt(m_rd());
std::random_device	Util::m_rd;
time_t				Util::m_OpenServerTimeSecond = 0;
struct tm			Util::m_tm = tm();
int					Util::m_day = 0;

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

long long Util::GetSysMilliseconds()
{
	auto time_now = std::chrono::system_clock::now();
	auto duration_in_ms = std::chrono::duration_cast<std::chrono::milliseconds>(time_now.time_since_epoch());
	return duration_in_ms.count();
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

const struct tm& Util::GetOpenServerTimeTM()
{
	return m_tm;
}

int Util::GetServiceDays()
{
	return m_day;
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

	// 开服时间时间戳
	m_tm = tm1;
	m_tm.tm_wday = 0;
	m_tm.tm_yday = 0;
	m_tm.tm_isdst = -1;
	tm1.tm_year -= 1900;
	tm1.tm_mon--;
	tm1.tm_isdst = -1;
	m_OpenServerTimeSecond = mktime(&tm1);

	// 开服天数
	time_t tick2 = ::time(nullptr);
	struct tm tm2;
	tm2 = *localtime(&tick2);
	m_day = tm2.tm_mday - m_tm.tm_mday + 1;

	return true;
}
