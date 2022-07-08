#pragma once

// �����
class Player
{
public:
	Player(const unsigned int& index, const TCPSocketInfo* pSockInfo, const long long& userId);
	virtual ~Player();

public:
	long long GetUserId() const;
	int GetIndex() { return m_index; }
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
	void SetPlayerPreproces(SubPlayerPreproces* pp) { m_SubPlayerPreproces = pp; }

public:
	// �ص�����
	void AddNetCallback(MsgCmd cmd, std::function<void(PlayerInfo*)>&& fun);
	void AddMysqlCallback(std::string&& name, std::function<void(std::string&)>&& fun);
	bool CallBackFun();
	bool CallBackFun(MsgCmd cmd, PlayerInfo* pPlayerInfo);

public:
	// ���ݿ����
	// ����һ�����ݿ�
	bool LoadOneSql(long long userId, std::string sqlName, CMysqlHelper::MysqlData& queryData, std::string dataStr = "data");
	// ���ض������ݿ�
	bool LoadMulitySql(std::string userId, std::string sqlName, CMysqlHelper::MysqlData& queryData, std::string dataStr = "data");
	// insert mysql
	void SaveInsertSQL(std::string sqlName, std::string name, std::string data, std::string keyName = "userid", std::string dataName = "data");
	// delete mysql
	void SaveDeleteSQL(std::string sqlName, const std::string& sCondition);
	// replace mysql
	void SaveReplaceSQL(std::string sqlName, std::string name, std::string data, long long userId = 0, std::string keyName = "userid", std::string dataName = "data");
	// update mysql
	void SaveUpdateSQL(std::string sqlName, std::string name, std::string data, const std::string& sCondition, std::string keyName = "userid", std::string dataName = "data");

private:
	unsigned int			m_index;				// �������
	PlayerAttrs				m_PlayerAttrs;			// �������
	long long				m_userId;				// ���id
	const TCPSocketInfo*	m_pTcpSockInfo;			// ���TCP��������Ϣ
	SubPlayerPreproces*		m_SubPlayerPreproces;	// ���Ԥ������
	NetFunMap				m_NetCBFunMap;			// �ص���������
	MysqlFunMap				m_MysqlCBFunMap;		// �ص���������
};