#include "../Game/stdafx.h"

// �½����ݿ��
void PlayerPrepClient::InitMysqlTable()
{
	RegisterCreate("useraccount", 1024);
	RegisterTable("serverlist", 1024);
	RegisterTable("move", 1024);
	RegisterTable("skill", 1024 * 2);
}
