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
	std::string				id;
	std::string				pw;

	bool GetConnect();
	const unsigned int& GetIndex();
	const TCPSocketInfo* GetSocketInfo();
	const uint64_t& getUserId();

	LoadPlayerKey(int nIndex, const TCPSocketInfo* sockInfo, std::string& sId, std::string& sPw) : index(nIndex), pSockInfo(sockInfo), userId(0), id(sId), pw(sPw) {}
	~LoadPlayerKey() {}
};

typedef std::list<UINT>												TimerList;		// ��ʱ��
typedef std::map<int, int>											AttrsMap;		// ����
typedef std::set<unsigned int>										OnLinePlayerSet;// �������
typedef std::list<std::string>										SqlList;		// ���ݿ����list<sql>	
typedef std::map<std::string, std::string>							SqlKeyDataMap;	// ���ݿ��ѯ���
typedef std::vector<std::function<void()>>							AttrsFunMap;	// ��Ϣ�ص�����
typedef std::map<TimerCmd, std::function<void()>>					TimerFunMap;	// ��Ϣ�ص�����
typedef std::vector<std::function<void(SocketCloseLine*)>>			ExitFunMap;		// ��Ϣ�ص�����
typedef std::map<MsgCmd, std::function<void(PlayerInfo*)>>			NetFunMap;		// ��Ϣ�ص�����
typedef std::map<std::string, std::function<void(std::string&)>>	MysqlFunMap;	// ��Ϣ�ص�����

// ע����Ϸ����ص�
#define RegisterAttrs(pobj, obj, name)\
if (!pobj)\
{\
	CLog::Write(LogMgr()->GetErrorLog().c_str(), LOG_CERROR, __FILE__, __LINE__, __FUNCTION__, "ע����Ϣʧ�� ����д��");\
}\
else\
{\
	pobj->AddAttrsCallback(std::move(std::bind(&name, obj)));\
}

// ע������Э��
#define RegisterNetwk(pobj, obj, name, cmd)\
if (!pobj)\
{\
	CLog::Write(LogMgr()->GetErrorLog().c_str(), LOG_CERROR, __FILE__, __LINE__, __FUNCTION__, "ע����Ϣʧ�� ����д��");\
}\
else\
{\
	pobj->AddNetCallback(cmd, std::move(std::bind(&name, obj, std::placeholders::_1)));\
}

// ע�����ݿ�ص�
#define RegisterMysql(pobj, obj, name, sql)\
if (!pobj)\
{\
	CLog::Write(LogMgr()->GetErrorLog().c_str(), LOG_CERROR, __FILE__, __LINE__, __FUNCTION__, "ע����Ϣʧ�� ����д��");\
}\
else\
{\
	pobj->AddMysqlCallback(sql, std::move(std::bind(&name, obj, std::placeholders::_1)));\
}

// ע�ᴴ�����ݿ�
#define RegisterCreat(pobj, name)\
if (!pobj)\
{\
	CLog::Write(LogMgr()->GetErrorLog().c_str(), LOG_CERROR, __FILE__, __LINE__, __FUNCTION__, "ע����Ϣʧ�� ����д��");\
}\
else\
{\
	pobj->CreateTableI(name);\
}

#define RegisterCreatS(pobj, name)\
if (!pobj)\
{\
	CLog::Write(LogMgr()->GetErrorLog().c_str(), LOG_CERROR, __FILE__, __LINE__, __FUNCTION__, "ע����Ϣʧ�� ����д��");\
}\
else\
{\
	pobj->CreateTableS(name);\
}

// ע�ᶨʱ��
#define RegisterTimer(pobj, obj, name, cmd, uElapse, timerType)\
if (!pobj)\
{\
	CLog::Write(LogMgr()->GetErrorLog().c_str(), LOG_CERROR, __FILE__, __LINE__, __FUNCTION__, "ע����Ϣʧ�� ����д��"); \
}\
else\
{\
	pobj->SetTimer(cmd, uElapse, timerType);\
	pobj->AddTimerCallback(cmd, std::move(std::bind(&name, obj)));\
}

// ��ע�ᶨʱ��
#define UnRegisterTimer(pobj, cmd)\
if (!pobj)\
{\
	CLog::Write(LogMgr()->GetErrorLog().c_str(), LOG_CERROR, __FILE__, __LINE__, __FUNCTION__, "ע����Ϣʧ�� ����д��"); \
}\
else\
{\
	pobj->KillTimer(cmd);\
	pobj->DelTimerCallback(cmd);\
}

// �������
#define AddAttributes(pobj, attrs)\
if (!pobj)\
{\
	CLog::Write(LogMgr()->GetErrorLog().c_str(), LOG_CERROR, __FILE__, __LINE__, __FUNCTION__, "�������ʧ��");\
}\
else\
{\
	pobj->AdditionAttributes(attrs);\
}

// ע����Ϸ�˳�
#define RegisterLgout(pobj, obj, name)\
if (!pobj)\
{\
	CLog::Write(LogMgr()->GetErrorLog().c_str(), LOG_CERROR, __FILE__, __LINE__, __FUNCTION__, "ע����Ϣʧ�� ����д��");\
}\
else\
{\
	pobj->AddExitCallback(std::move(std::bind(&name, obj, std::placeholders::_1)));\
}
