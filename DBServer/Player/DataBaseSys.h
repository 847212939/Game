#pragma once
 
enum class DataBaseSysMsgCmd
{
	cs_load_player			= 1,	// ����������ݿ�
	cs_load_global			= 2,	// ����ȫ�����ݿ�
	cs_load_login			= 3,	// ���ص�¼���ݿ�
	cs_save_replace_player	= 4,	// ����������ݿ�
	cs_save_replace_global	= 5,	// ����ȫ�����ݿ�
	cs_save_replace_login	= 6,	// ����ȫ�����ݿ�
	cs_create_global		= 7,	// ����ȫ�����ݿ�
	cs_create_player		= 8,	// ����������ݿ�
	cs_create_login			= 9,	// ������¼���ݿ�
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
	bool SaveReplaceLoginMysql(Netmsg& msg, PlayerInfo* playerInfo);
};
