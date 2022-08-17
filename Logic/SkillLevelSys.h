#pragma once
class PlayerClient;

enum class SkillLevelSysCmd
{
	cs_lv = 1,			// Éý¼¶
};

class SkillLevelSys
{
public:
	SkillLevelSys(PlayerClient* playerClient);
	~SkillLevelSys();

private:
	void LoadMysql(std::string& data);
	void CalAttrs();
	void EnterScene();
	void NetWork(PlayerInfo* playerInfo);
	void Exit(SocketCloseLine* socketCloseLine);

private:
	PlayerClient* m_PlayerClient;
};
