#pragma once

class PlayerClient : public Player
{
public:
	PlayerClient(const unsigned int& index, const uint64_t& userId);
	virtual ~PlayerClient();

public:
	MoveSys& GetMoveSys();

private:
	MoveSys m_MoveSys;
};
