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
	BuildReference(MoveSys, MoveSys)			// �ƶ�ϵͳ
	BuildReference(SkillLvSys, SkillLvSys)	// ���ܵȼ�ϵͳ
};
