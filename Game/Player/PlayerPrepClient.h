#pragma once

class PlayerPrepClient : public PlayerPrep
{
public:
	PlayerPrepClient();
	virtual~PlayerPrepClient();

public:
	// �������ݿ�
	void InitMysqlTable();
	void RegisterGlobalCallBack();

protected:
	PlayerPrepClient(const PlayerPrepClient& my);
	PlayerPrepClient& operator=(const PlayerPrepClient& my);

public:
	// ��ȡ����
	SceneClient* GetSceneClient();

public:
	// ��ͻ��˷��Ͳ������
	void SendOperateResults(SocketReadLine* pMsg, bool results, uint64_t userid);

private:
	SceneClient	m_SceneClient;

public:
	BuildReference(LoginSys, LoginSys)			// ��¼ϵͳ
	BuildReference(CrossSys, CrossSys)			// ���ϵͳ
};
