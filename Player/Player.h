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
	PlayerInfo* m_PlayerInfo;   // 玩家信息
	PlayerAttrs m_PlayerAttrs;  // 玩家属性
};