#pragma once

enum class DataBaseSysMsgCmd
{
	cs_load_player		= 1,	// 加载玩家数据库
	cs_load_global		= 2,	// 加载全局数据库
	cs_save_player		= 3,	// 保存玩家数据库
	cs_save_global		= 4,	// 保存全局数据库
	cs_create_global	= 5,	// 创建全局数据库
	cs_create_player	= 6,	// 创建玩家数据库
	cs_load_login		= 7,	// 加载登录数据库
};

class MysqlClient
{
public:
	MysqlClient();
	~MysqlClient();

public:
	static void LoadGlobalMysql(SLoadMysql& loadMysql);
	static void LoadLoginMysql(std::string& userid, SLoadMysql& loadMysql);
	static void LoadPlayerMysql(Player* player, SLoadMysql& loadMysql);

private:

};