#pragma once

// 玩家类
class Player
{
public:

public:
	Player(PlayerInfo* pPlayerInfo);
	~Player();

public:
	// 获取玩家id
	std::string GetUserId();
	// 获取玩家信息
	PlayerInfo* GetPlayerInfo();

private:
	// 玩家信息
	PlayerInfo* m_PlayerInfo;
};