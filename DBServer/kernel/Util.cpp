﻿#include "../stdafx.h"

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

long long Util::GetSysMilliseconds()
{
	auto time_now = std::chrono::system_clock::now();
	auto duration_in_ms = std::chrono::duration_cast<std::chrono::milliseconds>(time_now.time_since_epoch());
	return duration_in_ms.count();
}
uint64_t Util::GetCfgSecond(const VectorInt& vec)
{
	if (vec.size() < 3)
	{
		return 0;
	}

	return ((uint64_t)vec[0]) * 60 * 60 + ((uint64_t)vec[1]) * 60 + (uint64_t)vec[2];
}
uint64_t Util::GetCfgSecondEnd(const VectorInt& vec)
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

TCPClient* Util::GetTCPClient()
{
	return m_TCPClient;
}
void Util::Exit(bool& run)
{
	std::string str;

	while (run)
	{
		std::cin >> str;

		if (str == G_CfgMgr->GetCBaseCfgMgr().GetExit())
		{
			run = false;
			break;
		}
	}

	G_NetClient->Stop();
	G_NetClient->NotifyAll();
}

ListString Util::StringSplit(std::string& str, std::string separator, size_t count/* = 0*/)
{
	ListString listString;
	size_t pos = 0, cnt = 0;
	size_t size = str.size();

	for (size_t i = 0; i < size; i++)
	{
		// 最后一次拆分
		if (count > 0 && ++cnt >= count)
		{
			listString.emplace_back(str.substr(i, size - i));
			break;
		}
		pos = str.find(separator, i);
		if (pos == std::string::npos)
		{
			// 防止最后没有结尾分隔符保留
			if (i < size)
			{
				listString.emplace_back(str.substr(i, size - i));
			}
			break;
		}
		listString.emplace_back(str.substr(i, pos - i));
		i = pos + separator.size() - 1;
	}

	return listString;
}

