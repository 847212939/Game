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

bool LoadPlayerKey::GetConnect()
{
	if (!pSockInfo)
	{
		return false;
	}

	return pSockInfo->isConnect;
}

char PlayerPreproces::createptable[CreateTableLen] = "CREATE TABLE IF NOT EXISTS `%s` ("
"`userid` varchar(255) COLLATE utf8_unicode_ci NOT NULL,"
"`data` varchar(4096) COLLATE utf8_unicode_ci DEFAULT NULL,"
"PRIMARY KEY(`userid`) USING BTREE"
") ENGINE = MyISAM DEFAULT CHARSET = utf8 COLLATE = utf8_unicode_ci ROW_FORMAT = DYNAMIC;";
char PlayerPreproces::createpptable[CreateTableLen] = "CREATE TABLE IF NOT EXISTS `%s` ("
"`userid` bigint(20) NOT NULL,"
"`data` varchar(4096) COLLATE utf8_unicode_ci DEFAULT NULL,"
"PRIMARY KEY(`userid`) USING BTREE"
") ENGINE = MyISAM DEFAULT CHARSET = utf8 COLLATE = utf8_unicode_ci ROW_FORMAT = DYNAMIC; ";

const unsigned int& LoadPlayerKey::GetIndex()
{
	return index;
}

const TCPSocketInfo* LoadPlayerKey::GetSocketInfo()
{
	return pSockInfo;
}

const uint64_t& LoadPlayerKey::getUserId()
{
	return userId;
}
