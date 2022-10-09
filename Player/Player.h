#pragma once

// �����
class Player : virtual public Animal
{
public:
	Player(const unsigned int& index);
	virtual ~Player();

public:
	virtual AnimalType GetType();

protected:
	Player(const Player& my);
	Player& operator=(const Player& my);

public:
	// ��Ϣ����
	bool SendData(const char* pData, size_t size, MsgCmd mainID, int assistID, int handleCode, unsigned int uIdentification = 0);
	void MessageDispatch(MsgCmd cmd, PlayerInfo* playerInfo);

public:
	// �����ߴ���
	void CalAttrs();
	void LoadMysql();
	void EnterScene();
	void ExitGame(SocketCloseLine* pSocketClose);

public:
	void AttrsCallBackFun();
	void MysqlCallBackFun();
	void EnterSceneCallBackFun();
	void ExitCallBackFun(SocketCloseLine* pSocketClose);
	void NetCallBackFun(MsgCmd cmd, PlayerInfo* playerInfo);
	void AddAttrsCallback(std::function<void()>&& fun);
	void AddEnterSceneCallback(std::function<void()>&& fun);
	void AddExitCallback(std::function<void(SocketCloseLine*)>&& fun);
	void AddNetCallback(MsgCmd cmd, std::function<void(PlayerInfo*)>&& fun);
	void AddMysqlCallback(std::string name, std::function<void(std::string&)>&& fun);

public:
	// ���ݿ����
	void LoadOneSql(std::string sqlName, std::string& outStr, std::string dataStr = "data");
	void SaveInsertSQL(std::string sqlName, std::string data, std::string keyName = "userid", std::string dataName = "data");
	void SaveDeleteSQL(std::string sqlName, const std::string& sCondition);
	void SaveReplaceSQL(std::string sqlName, std::string data, std::string keyName = "userid", std::string dataName = "data");
	void SaveUpdateSQL(std::string sqlName, std::string data, const std::string& sCondition, std::string keyName = "userid", std::string dataName = "data");

public:
	GetSetMem(std::string, Playername)	// �������

	GetSetMem(bool, Load)				// ����Ƿ��ʼ�����
	GetSetMem(unsigned int, Index)		// ����Ƿ��ʼ�����

private:
	MapNetFun				m_NetCBFunMap;			// �ص���������
	VectorExitFunc			m_ExitFunMap;			// �ص���������
	MapMysqlFunc			m_MysqlCBFunMap;		// �ص���������
	VectorVoidFunc			m_AttrsFunMap;			// �ص���������
	VectorVoidFunc			m_EnterSceneFunMap;		// �ص���������
};