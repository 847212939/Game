#include "../Game/stdafx.h"

// 新建数据库表
void PlayerPrepClient::InitMysqlTable()
{
	RegisterLoginTable("useraccount", 1024);
	RegisterPlayerTable("serverlist", 1024);
	RegisterPlayerTable("move", 1024);
	RegisterPlayerTable("skill", 1024);
}
