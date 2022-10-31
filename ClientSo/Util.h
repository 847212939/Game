#pragma once

class Util
{
public:
	static Util* Instance();

private:
	Util();
	virtual~Util();

protected:
	// ���ÿ������캯��
	Util(const Util& my);
	Util& operator=(const Util& my);

public:
	// ��ȡ�����
	static unsigned int GetRandNum();
	// ��ȡ[A,B)�����,min<= ����� < iMax
	static int GetRandRange(int iMin, int iMax);

public:
	// �����˳�
	static void Exit(bool& run);
	// ����
	static char* Encrypt(char* content, size_t length);
	// ����
	static char* Decrypt(char* content, size_t length);
	static std::string& GetEncryptKey();
	static void SetEncryptKey(std::string key);

public:
	// ��ȡ���������
	static uint64_t GetSysSecond();
	// ��ȡ�����������
	static long long GetSysMilliseconds();
	// ��ȡ����ת��������
	static uint64_t GetCfgSecond(const VectorInt& vec);
	// ��ȡ����ת��������
	static uint64_t GetCfgSecondEnd(const VectorInt& vec);

public:
	/*
	* @brief �ַ�����ֺ���
	* @param str ԭʼ�ַ���
	* @param separator �ָ���
	* @param count ���Ϊ�����Ӵ�
	* @return ListString
	*/
	static ListString StringSplit(std::string& str, std::string separator, size_t count = 0);

public:
	// ΨһID
	uint64_t CreateUserId();
	TCPClient* GetTCPClient();

private:
	static std::random_device	m_rd;
	static std::mt19937			m_mt;
	TCPClient*					m_TCPClient;
	static time_t				m_OpenServerTimeSecond;
	static std::string			m_key;
};
