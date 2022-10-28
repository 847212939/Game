#include "../Game/stdafx.h"

// 新建数据库表
void PlayerPrepClient::InitMysqlTable()
{
	RegisterLoginTable("useraccount", 1024);
	RegisterPlayerTable("serverlist", 1024);
	RegisterPlayerTable("move", 1024);
	RegisterPlayerTable("skill", 1024);
}

// 注册玩家预处理数据库回调
void PlayerPrepClient::RegisterGlobalCallBack()
{
}

// 注册场景相关的数据库回调
void SceneClient::RegisterGlobalCallBack()
{
}

// 注册玩家中心相关的数据库回调
void PlayerCenterClient::RegisterGlobalCallBack()
{
}