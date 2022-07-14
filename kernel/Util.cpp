#include "../Game/stdafx.h"

COstringstream::COstringstream()
{
	m_os << std::hex;
}

COstringstream::~COstringstream()
{

}

COstringstream::operator std::string()
{
	return m_os.str();
}

std::string COstringstream::str()
{
	return m_os.str();
}

CIstringstream::~CIstringstream()
{

}

CIstringstream::CIstringstream(std::string str) : m_is(str), m_cnt(0)
{
	m_is >> std::hex;
}

CIstringstream& CIstringstream::operator >> (std::string& outStr)
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

CIstringstream& CIstringstream::operator >> (char* pBuf)
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

CIstringstream& CIstringstream::operator >> (unsigned char* pBuf)
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

// 方法类
std::mt19937		Util::m_mt(m_rd());
std::random_device	Util::m_rd;

Util* Util::Instance()
{
	static Util g_mgr;
	return &g_mgr;
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
	return m_IDGen;
}

uint64_t Util::CreateUserId()
{
	return m_IDGen.GenerateUID();
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
	time_t curSecond = time(NULL);
	if (curSecond != m_lastSecond)
	{
		m_lastSecond = curSecond;
		m_addID = 0;
	}

	return ((uint64_t)m_serverType << 59) + ((uint64_t)m_serverID << 52) + ((uint64_t)curSecond << 20) + ((uint64_t)m_addID++);
}

// 程序退出
void Util::Exit(bool& run, TCPClient& clien)
{
	std::string str;

	while (run)
	{
		std::cin >> str;

		if (str == "exit")
		{
			break;
		}
	}

	run = false;

	clien.SetRuninged(run);

	clien.NotifyAll();

	CBaseCfgMgr& baseCfgMgr = CfgMgr()->GetCBaseCfgMgr();
	int tm = baseCfgMgr.GetLogPrintTm();

	std::this_thread::sleep_for(std::chrono::seconds(tm + 10));
	exit(0);
}