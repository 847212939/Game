#pragma once
class PlayerClient;

enum class SkillLvSysCmd
{
	cs_lv = 1,			// Éý¼¶
};

class SkillLvSys
{
public:
	SkillLvSys(PlayerClient* playerClient);
	~SkillLvSys();

private:
	void CalAttrs();
	void EnterScene();
	void LoadMysql(std::string& data);
	void NetWork(PlayerInfo* playerInfo);
	void Exit(SocketCloseLine* socketCloseLine);

private:
	PlayerClient* m_PlayerClient;
};
