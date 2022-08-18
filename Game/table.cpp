#include "../Game/stdafx.h"

// 新建数据库表
void PlayerPrepClient::InitMysqlTable()
{
	RegisterCreatS("useraccount");
	RegisterCreat("move");
	RegisterCreatS("skill");
}
