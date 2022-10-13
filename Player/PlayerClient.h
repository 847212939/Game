#pragma once

class PlayerClient : public Player
{
public:
	PlayerClient(const unsigned int& index);
	virtual ~PlayerClient();

protected:
	PlayerClient(const PlayerClient& my);
	PlayerClient& operator=(const PlayerClient& my);

public:
	GetClass(MoveSys, MoveSys)			// 移动系统
	GetClass(SkillLvSys, SkillLvSys)	// 技能等级系统
};
