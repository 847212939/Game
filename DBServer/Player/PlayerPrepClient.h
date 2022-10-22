#pragma once

class PlayerPrepClient : public PlayerPrep
{
public:
	PlayerPrepClient();
	virtual~PlayerPrepClient();

protected:
	PlayerPrepClient(const PlayerPrepClient& my);
	PlayerPrepClient& operator=(const PlayerPrepClient& my);

public:
	// ��ȡ����
	SceneClient* GetSceneClient();

public:
	// ���Ͳ������
	void SendOperateResults(SocketReadLine* pMsg, bool results = true);

private:
	// �������ݿ�
	void InitMysqlTable();

private:
	SceneClient	m_SceneClient;

public:
	GetClass(LoginSys, LoginSys)	// ��¼ϵͳ
};
