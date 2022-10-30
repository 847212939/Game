#include "../stdafx.h"

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

// 对称加密
char* Util::Encrypt(char* content, size_t length)
{
	std::string sKey = G_BaseCfgMgr.GetKey();
	for (size_t i = 0; i < length; i++)
	{
		content[i] ^= sKey[i % (sKey.size() - 1)];
	}
	return content;
}
char* Util::Decrypt(char* content, size_t length)
{
	std::string sKey = G_BaseCfgMgr.GetKey();
	for (size_t i = 0; i < length; i++)
	{
		content[i] ^= sKey[i % (sKey.size() - 1)];
	}
	return content;
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
uint64_t Util::GetOpenServerTime()
{
	return m_OpenServerTimeSecond;
}
int Util::GetServiceDays()
{
	// 开服天数
	return (int)((::time(nullptr) - m_OpenServerTimeSecond) / (60 * 60 * 24)) + 1;
}
bool Util::InitTime()
{
	struct tm tm1;
	std::string openServerTime = G_BaseCfgMgr.GetOpenServerTime();
	if (sscanf(openServerTime.c_str(), "%4d-%2d-%2d %2d:%2d:%2d", 
		&tm1.tm_year, 
		&tm1.tm_mon, 
		&tm1.tm_mday, 
		&tm1.tm_hour, 
		&tm1.tm_min, 
		&tm1.tm_sec) <= 0 || 
		openServerTime.empty())
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

TCPClient* Util::GetTCPClient()
{
	return m_TCPClient;
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
void Util::Exit(bool& run)
{
	std::string str;

	while (run)
	{
		std::cin >> str;

		if (str == G_BaseCfgMgr.GetExit())
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
			listString.push_back(str.substr(i, size - i));
			break;
		}
		pos = str.find(separator, i);
		if (pos == std::string::npos)
		{
			// 防止最后没有结尾分隔符保留
			if (i < size)
			{
				listString.push_back(str.substr(i, size - i));
			}
			break;
		}
		listString.push_back(str.substr(i, pos - i));
		i = pos + separator.size() - 1;
	}

	return listString;
}

