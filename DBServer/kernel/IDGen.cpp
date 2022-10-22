#include "../stdafx.h"

IDGen::IDGen() :
	m_serverType(0),
	m_serverID(0),
	m_lastSecond(0),
	m_addID(0)
{

}

void IDGen::Init(int serverType, int serverID)
{
	m_serverType = serverType;
	m_serverID = serverID;
}

uint64_t IDGen::GenerateUID()
{
	time_t curSecond = time(nullptr);
	if (curSecond != m_lastSecond)
	{
		m_lastSecond = curSecond;
		m_addID = 0;
	}

	return ((uint64_t)m_serverType << 59) +
		((uint64_t)m_serverID << 52) +
		((uint64_t)curSecond << 20) +
		((uint64_t)m_addID++);
}
