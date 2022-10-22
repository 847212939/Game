#include "../Game/stdafx.h"

// 新建数据库表
void PlayerPrepClient::InitMysqlTable()
{
	RegisterCreate("useraccount", 1024);
	RegisterTable("serverlist", 1024);
	RegisterTable("move", 1024);
	RegisterTable("skill", 1024 * 2);
}
