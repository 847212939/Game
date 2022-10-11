#pragma once

class IDGen
{
public:
	IDGen();
	virtual ~IDGen() {}

protected:
	IDGen(const IDGen& my);
	IDGen& operator=(const IDGen& my);

public:
	void Init(int serverType, int serverID);
	uint64_t GenerateUID();

private:
	int         m_serverType;
	int	        m_serverID;
	time_t      m_lastSecond;
	uint64_t	m_addID;
};
