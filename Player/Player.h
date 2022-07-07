#pragma once

// �����
class Player
{
public:
	typedef std::map<MsgCmd, std::function<void(PlayerInfo*)>> PlayerCallBackFunMap;
public:
	Player(const unsigned int& index, const TCPSocketInfo* pSockInfo, const std::string& userId);
	virtual ~Player();

public:
	std::string GetUserId() const;
	int GetIndex() { return m_index; }
	const TCPSocketInfo* GetTCPSocketInfo();

public:
	// ��Ϣ����
	bool SendData(int index, void* pData, int size, int mainID, int assistID, int handleCode, void* pBufferevent, unsigned int uIdentification = 0);
	void DispatchMessage(MsgCmd cmd, PlayerInfo* pPlayerInfo);

public:
	// �������
	void LoadMysql();						
	void EnterGame();						
	bool EnterScene();
	void SetPlayerPreproces(SubPlayerPreproces* pp) { m_SubPlayerPreproces = pp; }

public:
	// �ص�����
	void AddCallBackFun(MsgCmd cmd, std::function<void(PlayerInfo*)>&& fun);
	bool CallBackFun(MsgCmd cmd, PlayerInfo* pPlayerInfo);

public:
	// ���ݿ����
	// ����һ�����ݿ�
	bool LoadOneSql(std::string userId, std::string sqlName, CMysqlHelper::MysqlData& queryData, std::string dataStr = "data");
	// ���ض������ݿ�
	bool LoadMulitySql(std::string userId, std::string sqlName, CMysqlHelper::MysqlData& queryData, std::string dataStr = "data");
	// insert mysql
	void SaveInsertSQL(std::string sqlName, std::string name, std::string data, std::string keyName = "userid", std::string dataName = "data");
	// delete mysql
	void SaveDeleteSQL(std::string sqlName, const std::string& sCondition);
	// replace mysql
	void SaveReplaceSQL(std::string sqlName, std::string name, std::string data, std::string keyName = "userid", std::string dataName = "data");
	// update mysql
	void SaveUpdateSQL(std::string sqlName, std::string name, std::string data, const std::string& sCondition, std::string keyName = "userid", std::string dataName = "data");

private:
	unsigned int			m_index;			// �������
	PlayerAttrs				m_PlayerAttrs;		// �������
	std::string				m_userId;			// ���id
	const TCPSocketInfo*	m_pTcpSockInfo;		// ���TCP��������Ϣ
	SubPlayerPreproces*		m_SubPlayerPreproces;	// ���Ԥ������
	PlayerCallBackFunMap	m_CallBackFunMap;	// �ص�����
};