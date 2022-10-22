#pragma once

template<typename... Parent>
struct Attrs : public Parent...
{
	int nAttack;		// ����
	int nDefense;		// ����
	int nMove;			// �ƶ�

	Attrs() : Parent()..., nAttack(0), nDefense(0), nMove(0)
	{}
	~Attrs(){}
};

// NPC����
struct NpcAttr
{
	NpcAttr(){}
	~NpcAttr(){}
};

// �������
struct PlayerAttr
{
	PlayerAttr(){}
	~PlayerAttr(){}
};

// ��������
struct MonsterAttr
{
	MonsterAttr(){}
	~MonsterAttr(){}
};

// ������
using NpcAttrs			= Attrs<NpcAttr>;
using PlayerAttrs		= Attrs<PlayerAttr>;
using MonsterAttrs		= Attrs<MonsterAttr>;
using AllParentAttrs	= Attrs<NpcAttr, PlayerAttr, MonsterAttr>;
