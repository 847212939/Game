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
	static Util* Instance();
	TCPClient* GetTCPClient();
	void InitCxxnet(pfCallBackEvent func);

public:
	// �ԳƼ���
	static uint8_t* Encrypt(uint8_t* content, int length);
	static uint8_t* Decrypt(uint8_t* content, int length);
	static long long GetSysMilliseconds();

private:
	TCPClient*	m_TCPClient;
};

class Cos
{
public:
	Cos();
	virtual ~Cos();

protected:
	Cos(const Cos& my);
	Cos& operator=(const Cos& my);

public:
	template<class T>
	Cos& operator << (T t);

	operator std::string();
	std::string str();

private:
	std::ostringstream m_os;
};

class Cis
{
public:
	virtual ~Cis();
	Cis(std::string str);

protected:
	Cis(const Cis& my);
	Cis& operator=(const Cis& my);

public:
	template<class T>
	Cis& operator >> (T& t);

	// ����ַ���
	Cis& operator >> (std::string& outStr);
	Cis& operator >> (char* pBuf);
	Cis& operator >> (unsigned char* pBuf);

private:
	// ��¼������ε�����
	size_t				m_cnt;
	std::istringstream	m_is;
};

template<class T>
Cos& Cos::operator << (T t)
{
	m_os << t << "\n";
	return *this;
}

template<class T>
Cis& Cis::operator >> (T& t)
{
	m_is >> t;
	++m_cnt;
	return *this;
}