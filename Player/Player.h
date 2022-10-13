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
	// ��һ����ҷ�����Ϣ
	bool SendData(const char* pData, size_t size, MsgCmd mainID, int assistID, int handleCode, unsigned int uIdentification = 0);
	// ��Ϣ�ɷ�
	void MessageDispatch(MsgCmd cmd, PlayerInfo* playerInfo);

public:
	// �����ߴ���
	// �������Լ���
	void CalAttrs();
	// ���߼������ݿ�
	void LoadMysql();
	// ���߽��볡��
	void EnterScene();
	// �����˳���Ϸ
	void ExitGame(SocketCloseLine* pSocketClose);

public:
	// ���Իص�����
	void AttrsCallBackFun();
	// ���ݿ�ص�����
	void MysqlCallBackFun();
	// ���볡���ص�����
	void EnterSceneCallBackFun();
	// �˳���Ϸ�ص�����
	void ExitCallBackFun(SocketCloseLine* pSocketClose);
	// ������Ϣ�ص�����
	void NetCallBackFun(MsgCmd cmd, PlayerInfo* playerInfo);
	// ������Իص�����
	void AddAttrsCallback(std::function<void()>&& fun);
	// ��ӽ��볡���ص�����
	void AddEnterSceneCallback(std::function<void()>&& fun);
	// ����˳���Ϸ�ص�����
	void AddExitCallback(std::function<void(SocketCloseLine*)>&& fun);
	// ���������Ϣ�ص�����
	void AddNetCallback(MsgCmd cmd, std::function<void(PlayerInfo*)>&& fun);
	// ������ݿ�ص�����
	void AddMysqlCallback(std::string name, std::function<void(std::string&)>&& fun);

public:
	// ���ݿ����
	void LoadOneSql(std::string sqlName, std::string& outStr, std::string dataStr = "data");
	void SaveInsertSQL(std::string sqlName, std::string data, std::string keyName = "userid", std::string dataName = "data");
	void SaveDeleteSQL(std::string sqlName, const std::string& sCondition);
	void SaveReplaceSQL(std::string sqlName, std::string data, std::string keyName = "userid", std::string dataName = "data");
	void SaveUpdateSQL(std::string sqlName, std::string data, const std::string& sCondition, std::string keyName = "userid", std::string dataName = "data");

public:
	// �������
	GetSetMem(std::string, Playername)
	// ����Ƿ��ʼ�����
	GetSetMem(bool, Load)
	// ����Ƿ��ʼ�����
	GetSetMem(unsigned int, Index)

private:
	// �ص���������
	MapNetFun				m_NetCBFunMap;	
	// �ص���������
	VectorExitFunc			m_ExitFunMap;	
	// �ص���������
	MapMysqlFunc			m_MysqlCBFunMap;
	// �ص���������
	VectorVoidFunc			m_AttrsFunMap;	
	// �ص���������
	VectorVoidFunc			m_EnterSceneFunMap;		
};