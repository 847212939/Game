#pragma once

class PlayerClient : public Player
{
public:
	PlayerClient(const unsigned int& index);
	virtual ~PlayerClient();

protected:
	PlayerClient(const PlayerClient& my);
	PlayerClient& operator=(const PlayerClient& my);

private:
	BuildReference(MoveSys, MoveSys)			// 移动系统
	BuildReference(SkillLvSys, SkillLvSys)		// 技能等级系统
	BuildReference(CrossClient, CrossClient)	// 跨服客户端
};
