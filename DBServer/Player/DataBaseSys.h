#pragma once
 
enum class DataBaseSysMsgCmd
{
	cs_load_player				= 1,	// 加载玩家数据库
	cs_load_global				= 2,	// 加载全局数据库
	cs_save_replace_player		= 3,	// 保存玩家数据库
	cs_save_replace_global		= 4,	// 保存全局数据库
	cs_create_global			= 5,	// 创建全局数据库
	cs_create_player			= 6,	// 创建玩家数据库
	cs_load_login				= 7,	// 加载登录数据库
	cs_create_login				= 8,	// 创建登录数据库
};

class DataBaseSys
{
public:
	DataBaseSys(PlayerPrepClient* ppc);
	~DataBaseSys();

private:
	void Network(PlayerInfo* playerInfo);

private:
	bool CreateLoginMysql(Netmsg& msg, PlayerInfo* playerInfo);
	bool CreateGlobalMysql(Netmsg& msg, PlayerInfo* playerInfo);
	bool CreatePlayerMysql(Netmsg& msg, PlayerInfo* playerInfo);

private:
	bool LoadPlayerMysql(Netmsg& msg, PlayerInfo* playerInfo);
	bool LoadLoginMysql(Netmsg& msg, PlayerInfo* playerInfo);
	bool LoadGlobalMysql(Netmsg& msg, PlayerInfo* playerInfo);

private:
	bool SaveReplaceGlobalMysql(Netmsg& msg, PlayerInfo* playerInfo);
	bool SaveReplacePlayerMysql(Netmsg& msg, PlayerInfo* playerInfo);

private:
	bool SaveMysql(Netmsg& msg, PlayerInfo* playerInfo);
};
