#pragma once

class PlayerClient : public Player
{
public:
	virtual ~PlayerClient();
	PlayerClient(const unsigned int& index, const TCPSocketInfo* pSockInfo, const uint64_t& userId, PlayerPrepClient* pp);

public:
	MoveSys& GetMoveSys() { return m_MoveSys; }

private:
	MoveSys m_MoveSys;
};