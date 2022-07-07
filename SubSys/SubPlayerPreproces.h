#pragma once

class SubPlayerPreproces : public PlayerPreproces
{
public:
	// 注册登录子协议
	enum class SubPPMsgCmd
	{
		CS_Login = 1,  // 登录
	};
public:
	~SubPlayerPreproces();
	SubPlayerPreproces(TCPClient* pTCPClient);

private:
	// 初始化消息回调函数
	void CallBackFunInit();

private:
	// 登录
	void LoginInAccount(PlayerInfo* pPlayerInfo);
	bool LoginIn(std::string& id, std::string& passwaed, PlayerInfo* pPlayerInfo);

private:
	std::string LoadUserId(std::string& id);
	std::string LoadUserAccount(std::string& id);

};
