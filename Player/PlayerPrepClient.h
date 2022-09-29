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
	SceneClient* GetSceneClient();

public:
	// 发送操作结果
	void SendOperateResults(SocketReadLine* pMsg);

private:
	// 建立数据库
	void InitMysqlTable();

private:
	SceneClient	m_SceneClient;

public:
	GetClass(LoginSys, LoginSys)
};
