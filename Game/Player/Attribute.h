#pragma once

template<typename... Parent>
struct Attrs : public Parent...
{
	int nAttack;		// 攻击
	int nDefense;		// 防御
	int nMove;			// 移动

	Attrs() : Parent()..., nAttack(0), nDefense(0), nMove(0)
	{}
	~Attrs(){}
};

// NPC属性
struct NpcAttr
{
	NpcAttr(){}
	~NpcAttr(){}
};

// 玩家属性
struct PlayerAttr
{
	PlayerAttr(){}
	~PlayerAttr(){}
};

// 怪物属性
struct MonsterAttr
{
	MonsterAttr(){}
	~MonsterAttr(){}
};

// 属性类
using NpcAttrs			= Attrs<NpcAttr>;
using PlayerAttrs		= Attrs<PlayerAttr>;
using MonsterAttrs		= Attrs<MonsterAttr>;
using AllParentAttrs	= Attrs<NpcAttr, PlayerAttr, MonsterAttr>;
