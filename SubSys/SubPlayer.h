#pragma once

class SubPlayer : public Player
{
public:
	virtual ~SubPlayer();
	SubPlayer(const unsigned int& index, const TCPSocketInfo* pSockInfo, const std::string& userId);

public:
	// 初始化回调函数
	void CallBackFunInit();

private:
	// 子系统
	bool Move(PlayerInfo* pPlayerInfo);

private:

};
