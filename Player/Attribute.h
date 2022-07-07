#pragma once

// ������
template<typename... Parent>
struct Attrs : public Parent...
{
	char id[64];

	Attrs() : Parent()..., id(""){}
	~Attrs() {}
};

// NPC����
struct NpcAttr
{
	char speack[128];	// ˵��

	NpcAttr() : speack("") {}
	~NpcAttr() {}
};

// �������
struct PlayerAttr
{
	int attack;		// ����
	int defense;	// ����

	PlayerAttr() : attack(0), defense(0) {}
	~PlayerAttr() {}
};

// ��������
struct MonsterAttr
{
	MonsterAttr() {}
	~MonsterAttr() {}
};

// ������
using NpcAttrs			= Attrs<NpcAttr>;
using PlayerAttrs		= Attrs<PlayerAttr>;
using MonsterAttrs		= Attrs<MonsterAttr>;
using AllParentAttrs	= Attrs<NpcAttr, PlayerAttr, MonsterAttr>;
