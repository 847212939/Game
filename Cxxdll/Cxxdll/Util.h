#pragma once

class Util
{
private:
	Util();
	virtual~Util();

protected:
	Util(const Util& my);
	Util& operator=(const Util& my);

public:
	// ∂‘≥∆º”√‹
	static char* Encrypt(char* content, size_t length);
	static char* Decrypt(char* content, size_t length);

public:
	static long long GetSysMilliseconds();

public:
	static Util* Instance();
	static std::string& GetEncryptKey();
	static void SetEncryptKey(std::string key);

public:
	TCPClient* GetTCPClient();

public:
	bool InitCxxnet(NetworkCallBackFunc netFunc, TimerCallBackFunc timerFunc, CloseCallBackFunc closeFunc);

private:
	TCPClient*	m_TCPClient;
	static std::string m_key;
};