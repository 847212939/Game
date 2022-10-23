#pragma once
class PlayerClient;

enum class SkillLvSysCmd
{
	cs_lv		= 1,			// 升级
	cs_load		= 2,			// 数据库加载
};

class SkillLvSys
{
public:
	SkillLvSys(PlayerClient* playerClient);
	~SkillLvSys();

private:
	void CalAttrs();
	void EnterScene();
	void NetWork(PlayerInfo* playerInfo);
	void Exit(SocketCloseLine* socketCloseLine);

	void LoadMysql(Netmsg& msg, PlayerInfo* playerInfo);

private:
	PlayerClient* m_PlayerClient;
};
