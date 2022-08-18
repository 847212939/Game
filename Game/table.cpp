#include "../Game/stdafx.h"

// 新建数据库表
void PlayerPrepClient::InitMysqlTable()
{
	RegisterCreate("useraccount");
	RegisterTable("move");
	RegisterTable("skill");
}
