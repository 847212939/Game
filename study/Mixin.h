#pragma once

// 第一次感受到泛型编程的魅力

// 混入编程
template<typename... Parent>
class Role : public Parent...
{
public:
	Role() : Parent()..., m_blood(0), m_muscle(0), m_bones(0), m_tendon(0){}
	~Role() {}

public:
	int m_blood;	// 血
	int m_muscle;	// 肌肉
	int m_bones;	// 骨骼
	int m_tendon;	// 筋
};

// NPC属性
class RoleNpcAttr
{
public:
	RoleNpcAttr() {}
	~RoleNpcAttr() {}

public:
	std::string m_speack;	// 说话
};

// 玩家属性
class RolePlayerAttr
{
public:
	RolePlayerAttr() : m_Id(0), m_attack(0), m_defense(0) {}
	~RolePlayerAttr() {}

public:
	int	m_Id;		// 玩家id
	int m_attack;	// 攻击
	int m_defense;	// 防御
};

// 怪物属性
class RoleMonsterAttr
{
public:
	RoleMonsterAttr(): m_Id(0){}
	~RoleMonsterAttr() {}

public:
	int m_Id;	// 怪物id
};

// 一个家族的类型
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