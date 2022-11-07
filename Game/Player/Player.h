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
	bool SendMsg(const char* pData, size_t size, MsgCmd mainID, int assistID, MsgCmd uIdentification = MsgCmd::MsgCmd_Begin, int handleCode = 0);
	// ��Ϣ�ɷ�
	void MessageDispatch(MsgCmd cmd, PlayerInfo* playerInfo);
private:
	bool SendLogicMsg(const char* pData, size_t size, MsgCmd mainID, int assistID, int handleCode, unsigned int uIdentification = 0);
	bool SendCrossMsg(const char* pData, size_t size, MsgCmd mainID, int assistID, int handleCode, unsigned int uIdentification = 0);

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
	void AddMysqlCallback(SLoadMysql loadMysql);

public:
	// �������
	BuildValue(std::string, Playername)
	// ����Ƿ��ʼ�����
	BuildValue(bool, Load)
	// ����Ƿ��ʼ�����
	BuildValue(unsigned int, Index)
	// ����߼���������
	BuildValue(unsigned int, LogicIndex)
	// serverid
	BuildValue(int, Serverid)

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