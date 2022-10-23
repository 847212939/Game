#pragma once
class PlayerClient;

enum class SkillLvSysCmd
{
	cs_lv		= 1,			// ����
	cs_load		= 2,			// ���ݿ����
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
