#pragma once

// �����
class Player
{
public:
	Player(const unsigned int& index, const TCPSocketInfo* pSockInfo, const uint64_t& userId, SubPlayerPreproces* pp);
	virtual ~Player();

public:
	bool GetLoad();
	int GetIndex();
	uint64_t GetUserId() const;
	const TCPSocketInfo* GetTCPSocketInfo();
	SubPlayerPreproces* GetSubPlayerPreproces();

public:
	// ��Ϣ����
	bool SendData(int index, const char* pData, size_t size, MsgCmd mainID, int assistID, int handleCode, void* pBufferevent, unsigned int uIdentification = 0);
	void DispatchMessage(MsgCmd cmd, PlayerInfo* pPlayerInfo);

public:
	// ���ߴ���
	void LoadMysql();
	void EnterGame();
	bool EnterScene();

public:
	// ���ߴ���
	void ExitGame();

public:
	// ���ݿ�����Ƿ����
	void SetLoad(bool load);

public:
	// �ص�����
	void AddAttrsCallback(std::function<void()>&& fun);
	void AddNetCallback(MsgCmd cmd, std::function<void(PlayerInfo*)>&& fun);
	void AddMysqlCallback(std::string name, std::function<void(std::string&&)>&& fun);
	void AttrsCallBackFun();
	void MysqlCallBackFun();
	void NetCallBackFun(MsgCmd cmd, PlayerInfo* pPlayerInfo);

public:
	// ����
	void AdditionAttributes(AttrsMap& attrs);
	void RefreshProperties();

public:
	// ���ݿ����
	std::string LoadOneSql(std::string sqlName, std::string dataStr = "data");
	void SaveInsertSQL(std::string sqlName, std::string data, std::string keyName = "userid", std::string dataName = "data");
	void SaveDeleteSQL(std::string sqlName, const std::string& sCondition);
	void SaveReplaceSQL(std::string sqlName, std::string data, std::string keyName = "userid", std::string dataName = "data");
	void SaveUpdateSQL(std::string sqlName, std::string data, const std::string& sCondition, std::string keyName = "userid", std::string dataName = "data");

private:
	bool					m_load;					// ����Ƿ��ʼ�����
	unsigned int			m_index;				// �������
	uint64_t				m_userId;				// ���id
	AttrsMap				m_AttrsMap;				// ���Լ���
	NetFunMap				m_NetCBFunMap;			// �ص���������
	AttrsFunMap				m_AttrsFunMap;			// �ص���������
	MysqlFunMap				m_MysqlCBFunMap;		// �ص���������
	SubPlayerPreproces*		m_SubPlayerPreproces;	// ���Ԥ������
	const TCPSocketInfo*	m_pTcpSockInfo;			// ���TCP��������Ϣ
};