#pragma once

class SubPlayer : public Player
{
public:
	virtual ~SubPlayer();
	SubPlayer(const unsigned int& index, const TCPSocketInfo* pSockInfo, const uint64_t& userId, SubPlayerPreproces* pp);

public:
	MoveSys& GetMoveSys() { return m_MoveSys; }

private:
	MoveSys m_MoveSys;
};
