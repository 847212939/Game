#pragma once

class SubPlayer : public Player
{
public:
	virtual ~SubPlayer();
	SubPlayer(const unsigned int& index, const TCPSocketInfo* pSockInfo, const std::string& userId);

public:
	// ��ʼ���ص�����
	void CallBackFunInit();

private:
	// ��ϵͳ
	bool Move(PlayerInfo* pPlayerInfo);

private:

};
