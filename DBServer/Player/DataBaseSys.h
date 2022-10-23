#pragma once
 
enum class DataBaseSysMsgCmd
{
	cs_load		= 1,	// 加载数据库
	cs_save		= 2,	// 保存数据库
	cs_create	= 3,	// 创建数据库
};

class DataBaseSys
{
public:
	DataBaseSys(PlayerPrepClient* ppc);
	~DataBaseSys();

private:
	void Network(PlayerInfo* playerInfo);

private:
	bool LoadMysql(PlayerInfo* playerInfo);
	bool SaveMysql(PlayerInfo* playerInfo);
	bool CreateMysql(PlayerInfo* playerInfo);
};
