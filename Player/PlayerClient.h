#pragma once

class PlayerClient : public Player
{
public:
	PlayerClient(const unsigned int& index, const uint64_t& userId);
	virtual ~PlayerClient();

protected:
	PlayerClient(const PlayerClient& my);
	PlayerClient& operator=(const PlayerClient& my);

public:
	MoveSys& GetMoveSys();

private:
	MoveSys m_MoveSys;
};
