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
	cs_create_login			= 8,	// ������¼���ݿ�
	cs_save_replace_login	= 9,	// ����ȫ�����ݿ�
};

class MysqlClient
{
public:
	MysqlClient();
	~MysqlClient();

public:
	// �������ݿ��
	static void CreateLoginMysql(std::string name, int cnt = 4096);
	static void CreateGlobalMysql(std::string name, int cnt = 4096);
	static void CreatePlayerMysql(std::string name, int cnt = 4096);

public:
	// �������ݿ�
	static void LoadGlobalMysql(SLoadMysql& loadMysql);
	static void LoadPlayerMysql(uint64_t userid, SLoadMysql& loadMysql);
	static void LoadLoginMysql(std::string& userid, SLoadMysql loadMysql);

public:
	// �������ݿ�
	static void SaveReplaceGlobalMysql(std::string sqlName, std::string&& data);
	static void SaveReplacePlayerMysql(uint64_t userid, std::string sqlName, std::string&& data);
	static void SaveReplaceLoginMysql(std::string& userid, std::string sqlName, std::string&& data);

private:

};