#pragma once

enum class DataBaseSysMsgCmd
{
	cs_load_player			= 1,	// 加载玩家数据库
	cs_load_global			= 2,	// 加载全局数据库
	cs_load_login			= 3,	// 加载登录数据库
	cs_save_replace_player	= 4,	// 保存玩家数据库
	cs_save_replace_global	= 5,	// 保存全局数据库
	cs_save_replace_login	= 6,	// 保存全局数据库
	cs_create_global		= 7,	// 创建全局数据库
	cs_create_player		= 8,	// 创建玩家数据库
	cs_create_login			= 9,	// 创建登录数据库
};

class MysqlClient
{
public:
	MysqlClient();
	~MysqlClient();

public:
	// 创建数据库表
	static void CreateLoginMysql(std::string name, int cnt = 4096);
	static void CreateGlobalMysql(std::string name, int cnt = 4096);
	static void CreatePlayerMysql(std::string name, int cnt = 4096);

public:
	// 加载数据库
	static void LoadGlobalMysql(SLoadMysql& loadMysql);
	static void LoadPlayerMysql(uint64_t userid, SLoadMysql& loadMysql);
	static void LoadLoginMysql(std::string& userid, SLoadMysql loadMysql);

public:
	// 保存数据库
	static void SaveReplaceGlobalMysql(std::string sqlName, std::string&& data);
	static void SaveReplacePlayerMysql(uint64_t userid, std::string sqlName, std::string&& data);
	static void SaveReplaceLoginMysql(std::string& userid, std::string sqlName, std::string&& data);
};