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

class DataBaseSys
{
public:
	DataBaseSys(PlayerPrepClient* ppc);
	~DataBaseSys();

private:
	void Network(PlayerInfo* playerInfo);

private:
	bool CreateLoginMysql(PlayerInfo* playerInfo);
	bool CreateGlobalMysql(PlayerInfo* playerInfo);
	bool CreatePlayerMysql(PlayerInfo* playerInfo);

private:
	bool LoadPlayerMysql(PlayerInfo* playerInfo);
	bool LoadLoginMysql(PlayerInfo* playerInfo);
	bool LoadGlobalMysql(PlayerInfo* playerInfo);

private:
	bool SaveReplaceGlobalMysql(PlayerInfo* playerInfo);
	bool SaveReplacePlayerMysql(PlayerInfo* playerInfo);
	bool SaveReplaceLoginMysql(PlayerInfo* playerInfo);
};
