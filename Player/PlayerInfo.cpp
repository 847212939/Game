#include "../Game/stdafx.h"

PlayerInfo::PlayerInfo() :
	m_pMsg(nullptr), 
	m_pData(nullptr),
	m_uSrverType(ServiceType::SERVICE_TYPE_LOGIC),
	m_userId(0),
	m_pTcpSockInfo(nullptr)
{

}

PlayerInfo::~PlayerInfo()
{

}

const bool& LoadPlayerKey::GetConnect()
{
	return pSockInfo->isConnect;
}

const unsigned int& LoadPlayerKey::GetIndex()
{
	return index;
}

const TCPSocketInfo* LoadPlayerKey::GetSocketInfo()
{
	return pSockInfo;
}

const long long& LoadPlayerKey::getUserId()
{
	return userId;
}