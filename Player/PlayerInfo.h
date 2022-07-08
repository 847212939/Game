#pragma once

// �����Ϣ
class PlayerInfo
{
public:
	PlayerInfo();
	virtual~PlayerInfo();

public:
	
	SocketReadLine* m_pMsg;					// SOCKET��ȡ֪ͨ�ṹ����
	void*			m_pData;				// ��ҷ��͹���������
	ServiceType		m_uSrverType;			// ����������
	long long		m_userId;				// ���id
	const TCPSocketInfo* m_pTcpSockInfo;	// ���TCP��������Ϣ
};

struct LoadPlayerKey
{
	unsigned int			index;
	const TCPSocketInfo*	pSockInfo;
	long long				userId;

	const bool& GetConnect();
	const unsigned int& GetIndex();
	const TCPSocketInfo* GetSocketInfo();
	const long long& getUserId();

	LoadPlayerKey(int nIndex, const TCPSocketInfo* sockInfo, long long& Id) : index(nIndex), pSockInfo(sockInfo), userId(Id) {}
	~LoadPlayerKey() {}
};

typedef std::map<std::string, std::string>							SqlKeyDataMap;	// ���ݿ��ѯ���
typedef std::map<MsgCmd, std::function<void(PlayerInfo*)>>			NetFunMap;		// ��Ϣ�ص�����
typedef std::map<std::string, std::function<void(std::string&)>>	MysqlFunMap;	// ��Ϣ�ص�����
