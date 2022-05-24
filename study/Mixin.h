#pragma once

// ��һ�θ��ܵ����ͱ�̵�����

// ������
template<typename... Parent>
class Role : public Parent...
{
public:
	Role() : Parent()..., m_blood(0), m_muscle(0), m_bones(0), m_tendon(0){}
	~Role() {}

public:
	int m_blood;	// Ѫ
	int m_muscle;	// ����
	int m_bones;	// ����
	int m_tendon;	// ��
};

// NPC����
class RoleNpcAttr
{
public:
	RoleNpcAttr() {}
	~RoleNpcAttr() {}

public:
	std::string m_speack;	// ˵��
};

// �������
class RolePlayerAttr
{
public:
	RolePlayerAttr() : m_Id(0), m_attack(0), m_defense(0) {}
	~RolePlayerAttr() {}

public:
	int	m_Id;		// ���id
	int m_attack;	// ����
	int m_defense;	// ����
};

// ��������
class RoleMonsterAttr
{
public:
	RoleMonsterAttr(): m_Id(0){}
	~RoleMonsterAttr() {}

public:
	int m_Id;	// ����id
};

// һ�����������
template<typename T>
class Family
{
public:
	Family() {}
	~Family() {}

public:
	std::vector<T> m_familyVec;
};

using RoleNpc = Role<RoleNpcAttr>;
using RolePlayer = Role<RolePlayerAttr>;
using RoleMonster = Role<RoleMonsterAttr>;
using AllParent = Role<RoleNpcAttr, RolePlayerAttr, RoleMonsterAttr>;

using RoleNpcFamily = Family<RoleNpc>;
using RolePlayerFamily = Family<RolePlayer>;
using RoleMonsterFamily = Family<RoleMonster>;
using AllParentFamily = Family<AllParent>;

void TestFun();