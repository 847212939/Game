#pragma once
 
enum class DataBaseSysMsgCmd
{
	cs_load = 1,	// º”‘ÿ
	cs_save = 2,	// ±£¥Ê
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
};
