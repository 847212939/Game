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