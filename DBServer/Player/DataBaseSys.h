#pragma once
 
enum class DataBaseSysMsgCmd
{
	cs_load_player				= 1,	// ����������ݿ�
	cs_load_global				= 2,	// ����ȫ�����ݿ�
	cs_save_replace_player		= 3,	// ����������ݿ�
	cs_save_replace_global		= 4,	// ����ȫ�����ݿ�
	cs_create_global			= 5,	// ����ȫ�����ݿ�
	cs_create_player			= 6,	// ����������ݿ�
	cs_load_login				= 7,	// ���ص�¼���ݿ�
};

class DataBaseSys
{
public:
	DataBaseSys(PlayerPrepClient* ppc);
	~DataBaseSys();

private:
	void Network(PlayerInfo* playerInfo);

private:
	bool LoadPlayerMysql(Netmsg& msg, PlayerInfo* playerInfo);
	bool LoadLoginMysql(Netmsg& msg, PlayerInfo* playerInfo);
	bool LoadGlobalMysql(Netmsg& msg, PlayerInfo* playerInfo);

private:
	bool SaveGlobalMysql(Netmsg& msg, PlayerInfo* playerInfo);
	bool SaveReplacePlayerMysql(Netmsg& msg, PlayerInfo* playerInfo);

private:
	bool SaveMysql(Netmsg& msg, PlayerInfo* playerInfo);
	bool CreateMysql(Netmsg& msg, PlayerInfo* playerInfo);
};
