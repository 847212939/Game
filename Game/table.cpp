#include "../Game/stdafx.h"

// �½����ݿ��
void PlayerPrepClient::InitMysqlTable()
{
	RegisterLoginTable("useraccount", 1024);
	RegisterPlayerTable("serverlist", 1024);
	RegisterPlayerTable("move", 1024);
	RegisterPlayerTable("skill", 1024);

	Log(CINF, "�������ݿ�ɹ�");
}

// ע�����Ԥ�������ݿ�ص�
void PlayerPrepClient::RegisterGlobalCallBack()
{
	Log(CINF, "ע�����Ԥ�������ȫ�����ݿ�ɹ�");
}

// ע�᳡����ص����ݿ�ص�
void SceneClient::RegisterGlobalCallBack()
{
	Log(CINF, "ע�᳡�����ȫ�����ݿ�ɹ�");
}

// ע�����������ص����ݿ�ص�
void PlayerCenterClient::RegisterGlobalCallBack()
{
	Log(CINF, "ע����ҹ���ȫ�����ݿ�ɹ�");
}