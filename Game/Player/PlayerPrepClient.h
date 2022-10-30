#pragma once

class PlayerPrepClient : public PlayerPrep
{
public:
	PlayerPrepClient();
	virtual~PlayerPrepClient();

public:
	// 建立数据库
	void InitMysqlTable();
	void RegisterGlobalCallBack();

protected:
	PlayerPrepClient(const PlayerPrepClient& my);
	PlayerPrepClient& operator=(const PlayerPrepClient& my);

public:
	// 获取场景
	SceneClient* GetSceneClient();

public:
	// 发送操作结果
	void SendOperateResults(SocketReadLine* pMsg, bool results = true);

private:
	SceneClient	m_SceneClient;

public:
	BuildReference(LoginSys, LoginSys)	// 登录系统
};
