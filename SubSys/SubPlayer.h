#pragma once

class SubPlayer : public Player
{
public:
	virtual ~SubPlayer();
	SubPlayer(const unsigned int& index, const TCPSocketInfo* pSockInfo, const std::string& userId);

public:
	MoveSys& GetMoveSys() { return m_MoveSys; }

private:
	MoveSys m_MoveSys;
};
