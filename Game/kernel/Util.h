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
	static int GetRandNum();
	// ��ȡ[A,B)�����,min<= ����� < iMax
	static int GetRandRange(int iMin, int iMax);
	// �����˳�
	static void Exit(bool& run);
	// ����
	static char* Encrypt(char* content, size_t length);
	// ����
	static char* Decrypt(char* content, size_t length);
	// ��ȡ���������
	static uint64_t GetSysSecond();
	// ��ȡ�����������
	static long long GetSysMilliseconds();
	// ��ȡ����ת��������
	static uint64_t GetCfgSecond(const VectorInt& vec);
	// ��ȡ����ת��������
	static uint64_t GetCfgSecondEnd(const VectorInt& vec);
	// �������û�ȡ��������
	static int GetServiceDays();
	// ��ȡ���������ڵ�����
	static uint64_t GetOpenServerTime();
	// ��������ʱ���ʼ��ʱ��
	static bool InitTime();
	// ������
	static Animal* CreatAnimal(AnimalType type, int mid = 0);
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
	IDGen& GetIDGen();
	uint64_t CreateUserId();
	TCPClient* GetTCPClient();

private:
	static std::random_device	m_rd;
	static std::mt19937			m_mt;
	IDGen*						m_IDGen;
	TCPClient*					m_TCPClient;
	static time_t				m_OpenServerTimeSecond;
};
