#pragma once

class SubPlayer : public Player
{
public:
	SubPlayer(const unsigned int& index, const TCPSocketInfo* pSockInfo, const std::string& userId);
	virtual ~SubPlayer();

public:
	// ��ʼ���ص�����
	void CallBackFunInit();

private:
	// ��ϵͳ
	bool Move(PlayerInfo* pPlayerInfo);

private:

};
