#include "../Game/stdafx.h"

// �½����ݿ��
void PlayerPrepClient::InitMysqlTable()
{
	RegisterCreate("useraccount");
	RegisterTable("move");
	RegisterTable("skill");
}
