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
	// 向客户端发送操作结果
	void SendOperateResults(SocketReadLine* pMsg, bool results, uint64_t userid);

private:
	SceneClient	m_SceneClient;

public:
	BuildReference(LoginSys, LoginSys)			// 登录系统
	BuildReference(CrossSys, CrossSys)			// 跨服系统
};
