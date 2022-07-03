#pragma once

// 混入编程
template<typename... Parent>
struct Attrs : public Parent...
{
	char id[64];

	Attrs() : Parent()..., id(""){}
	~Attrs() {}
};

// NPC属性
struct NpcAttr
{
	char speack[128];	// 说话

	NpcAttr() : speack("") {}
	~NpcAttr() {}
};

// 玩家属性
struct PlayerAttr
{
	int attack;		// 攻击
	int defense;	// 防御

	PlayerAttr() : attack(0), defense(0) {}
	~PlayerAttr() {}
};

// 怪物属性
struct MonsterAttr
{
	MonsterAttr() {}
	~MonsterAttr() {}
};

// 属性类
using NpcAttrs			= Attrs<NpcAttr>;
using PlayerAttrs		= Attrs<PlayerAttr>;
using MonsterAttrs		= Attrs<MonsterAttr>;
using AllParentAttrs	= Attrs<NpcAttr, PlayerAttr, MonsterAttr>;
