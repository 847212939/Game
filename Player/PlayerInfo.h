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
	uint64_t				userId;
	const TCPSocketInfo*	pSockInfo;

	const bool& GetConnect();
	const unsigned int& GetIndex();
	const TCPSocketInfo* GetSocketInfo();
	const uint64_t& getUserId();

	LoadPlayerKey(int nIndex, const TCPSocketInfo* sockInfo, uint64_t& Id) : index(nIndex), pSockInfo(sockInfo), userId(Id) {}
	~LoadPlayerKey() {}
};

typedef std::map<int, int>											AttrsMap;		// ����
typedef std::map<std::string, std::string>							SqlKeyDataMap;	// ���ݿ��ѯ���
typedef std::vector<std::function<void(AttrsMap&)>>					AttrsFunMap;	// ��Ϣ�ص�����
typedef std::map<MsgCmd, std::function<void(PlayerInfo*)>>			NetFunMap;		// ��Ϣ�ص�����
typedef std::map<std::string, std::function<void(std::string&&)>>	MysqlFunMap;	// ��Ϣ�ص�����

// ����ע�ắ��
#define RegisterAttrs(pobj, obj, name)		if(pobj) pobj->AddAttrsCallback(std::move(std::bind(&name, obj, std::placeholders::_1)))
#define RegisterNetwk(pobj, obj, name, cmd)	if(pobj) pobj->AddNetCallback(cmd, std::move(std::bind(&name, obj, std::placeholders::_1)))
#define RegisterMysql(pobj, obj, name, sql)	if(pobj) pobj->AddMysqlCallback(sql, std::move(std::bind(&name, obj, std::placeholders::_1)))
