#include "../Game/stdafx.h"

PlayerInfo::PlayerInfo() :
	m_pMsg(nullptr), 
	m_pData(nullptr),
	m_uSrverType(ServiceType::SERVICE_TYPE_LOGIC),
	m_userId(""),
	m_pTcpSockInfo(nullptr)
{

}

PlayerInfo::~PlayerInfo()
{

}
