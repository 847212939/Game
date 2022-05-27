#include "../Game/stdafx.h"

void TEST::TestFun()
{
	RoleNpc roleNpc;
	roleNpc.m_speack = "ÄãÊÇÉµ±Æ";

	RoleNpcFamily roleNpcFamily;
	roleNpcFamily.m_familyVec.push_back(roleNpc);

}

void TEST::TestFun1()
{
	std::vector<int> a;
	std::vector<int> b;

	std::vector<int> c = a + b;
}