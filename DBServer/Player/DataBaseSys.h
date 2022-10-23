#pragma once
 
enum class DataBaseSysMsgCmd
{
	cs_load		= 1,	// �������ݿ�
	cs_save		= 2,	// �������ݿ�
	cs_create	= 3,	// �������ݿ�
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
