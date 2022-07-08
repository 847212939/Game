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
	uint64_t		m_userId;				// ���id
	const TCPSocketInfo* m_pTcpSockInfo;	// ���TCP��������Ϣ
};

struct LoadPlayerKey
{
	unsigned int			index;
	const TCPSocketInfo*	pSockInfo;
	uint64_t				userId;

	const bool& GetConnect();
	const unsigned int& GetIndex();
	const TCPSocketInfo* GetSocketInfo();
	const uint64_t& getUserId();

	LoadPlayerKey(int nIndex, const TCPSocketInfo* sockInfo, uint64_t& Id) : index(nIndex), pSockInfo(sockInfo), userId(Id) {}
	~LoadPlayerKey() {}
};

typedef std::map<std::string, std::string>							SqlKeyDataMap;	// ���ݿ��ѯ���
typedef std::map<MsgCmd, std::function<void(PlayerInfo*)>>			NetFunMap;		// ��Ϣ�ص�����
typedef std::map<std::string, std::function<void(std::string&&)>>	MysqlFunMap;	// ��Ϣ�ص�����
