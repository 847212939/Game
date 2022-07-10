#include "../Game/stdafx.h"

char createpptable[CreateTableLen] = "CREATE TABLE IF NOT EXISTS `%s` ("
"`userid` varchar(255) COLLATE utf8_unicode_ci NOT NULL,"
"`data` varchar(1024) COLLATE utf8_unicode_ci DEFAULT NULL,"
"PRIMARY KEY(`userid`) USING BTREE"
") ENGINE = MyISAM DEFAULT CHARSET = utf8 COLLATE = utf8_unicode_ci ROW_FORMAT = DYNAMIC;";

char createptable[CreateTableLen] = "CREATE TABLE IF NOT EXISTS `%s` ("
"`userid` bigint(20) NOT NULL,"
"`data` varchar(1024) COLLATE utf8_unicode_ci DEFAULT NULL,"
"PRIMARY KEY(`userid`) USING BTREE"
") ENGINE = MyISAM DEFAULT CHARSET = utf8 COLLATE = utf8_unicode_ci ROW_FORMAT = DYNAMIC; ";

SubPlayerPreproces::SubPlayerPreproces(TCPClient* pTCPClient) :
	PlayerPreproces(pTCPClient),
	m_LoginSys(this)
{
	Register();
}

SubPlayerPreproces::~SubPlayerPreproces()
{
}

void SubPlayerPreproces::Register()
{
	RegisterCreat(this, "move");
	RegisterCreat(this, "useraccount");
}

void SubPlayerPreproces::CreateTable(std::string name)
{
	char sql[CreateTableLen] = "";

	sprintf_s(sql, CreateTableLen, createpptable, name.c_str());

	CreateTableSql(sql);
}

LoginSys& SubPlayerPreproces::GetLoginSys()
{
	return m_LoginSys;
}