#include "../Game/stdafx.h"

// �½����ݿ��
void PlayerPrepClient::InitMysqlTable()
{
	RegisterLoginTable("useraccount", 1024);
	RegisterPlayerTable("serverlist", 1024);
	RegisterPlayerTable("move", 1024);
	RegisterPlayerTable("skill", 1024);
}

// ע�����Ԥ�������ݿ�ص�
void PlayerPrepClient::RegisterGlobalCallBack()
{
}

// ע�᳡����ص����ݿ�ص�
void SceneClient::RegisterGlobalCallBack()
{
}

// ע�����������ص����ݿ�ص�
void PlayerCenterClient::RegisterGlobalCallBack()
{
}