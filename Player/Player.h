#pragma once

// �����
class Player
{
public:
	Player(const unsigned int& index, const TCPSocketInfo* pSockInfo, const uint64_t& userId);
	virtual ~Player();

public:
	bool GetLoad();
	int GetIndex();
	uint64_t GetUserId() const;
	const TCPSocketInfo* GetTCPSocketInfo();

public:
	// ��Ϣ����
	bool SendData(int index, void* pData, int size, int mainID, int assistID, int handleCode, void* pBufferevent, unsigned int uIdentification = 0);
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
	void SetPlayerPreproces(SubPlayerPreproces* pp);
	void SetLoad(bool load);

public:
	// �ص�����
	void AddNetCallback(MsgCmd cmd, std::function<void(PlayerInfo*)>&& fun);
	void AddMysqlCallback(std::string name, std::function<void(std::string&&)>&& fun);
	bool CallBackFun();
	bool CallBackFun(MsgCmd cmd, PlayerInfo* pPlayerInfo);

public:
	// ���ݿ����
	// ����һ�����ݿ�
	std::string LoadOneSql(std::string sqlName, uint64_t userId, std::string dataStr = "data");
	// insert mysql
	void SaveInsertSQL(std::string sqlName, uint64_t userId, std::string data, std::string keyName = "userid", std::string dataName = "data");
	// delete mysql
	void SaveDeleteSQL(std::string sqlName, const std::string& sCondition);
	// replace mysql
	void SaveReplaceSQL(std::string sqlName, uint64_t userId, std::string data, std::string keyName = "userid", std::string dataName = "data");
	// update mysql
	void SaveUpdateSQL(std::string sqlName, uint64_t userId, std::string data, const std::string& sCondition, std::string keyName = "userid", std::string dataName = "data");

private:
	bool					m_loadMysqled;			// ���ݿ������ɱ�־
	unsigned int			m_index;				// �������
	PlayerAttrs				m_PlayerAttrs;			// �������
	uint64_t				m_userId;				// ���id
	const TCPSocketInfo*	m_pTcpSockInfo;			// ���TCP��������Ϣ
	SubPlayerPreproces*		m_SubPlayerPreproces;	// ���Ԥ������
	NetFunMap				m_NetCBFunMap;			// �ص���������
	MysqlFunMap				m_MysqlCBFunMap;		// �ص���������
};