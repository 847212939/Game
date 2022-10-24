#pragma once

enum class DataBaseSysMsgCmd
{
	cs_load_player			= 1,	// ����������ݿ�
	cs_load_global			= 2,	// ����ȫ�����ݿ�
	cs_save_replace_player	= 3,	// ����������ݿ�
	cs_save_replace_global	= 4,	// ����ȫ�����ݿ�
	cs_create_global		= 5,	// ����ȫ�����ݿ�
	cs_create_player		= 6,	// ����������ݿ�
	cs_load_login			= 7,	// ���ص�¼���ݿ�
};

class Player;
class MysqlClient
{
public:
	MysqlClient();
	~MysqlClient();

public:
	static void LoadGlobalMysql(SLoadMysql& loadMysql);
	static void LoadLoginMysql(std::string& userid, SLoadMysql loadMysql);
	static void LoadPlayerMysql(Player* player, SLoadMysql& loadMysql);

public:
	static void SaveReplacePlayerMysql(Player* player, std::string sqlName, std::string&& data);
	static void SaveGlobalPlayerMysql(std::string sqlName, std::string data);

private:

};