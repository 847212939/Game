#include "../Game/stdafx.h"

// �½����ݿ��
void PlayerPrepClient::InitMysqlTable()
{
	RegisterCreate("useraccount");
	RegisterTable("serverlist");
	RegisterTable("move");
	RegisterTable("skill");
}
