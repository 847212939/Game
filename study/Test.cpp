#include "../Game/stdafx.h"

void TEST::TestFun()
{
	// ģ�����ʵ�ֶ������ļ̳�
	RoleNpc roleNpc;
	roleNpc.m_speack = "����ɵ��";

	RoleNpcFamily roleNpcFamily;
	roleNpcFamily.m_familyVec.push_back(roleNpc);

}

void TEST::TestFun1()
{
	std::vector<int> a;
	std::vector<double> b;

	// ʵ��std::vector�ӷ�
	auto c = a + b;
}

void TEST::TestFun2()
{
	std::conditional<true, int, std::string>::type c = 10;
	std::conditional<false, int, std::string>::type s = "123456";

	// �����ڼ�ʵ�ַ�֧�߼�
	// constexpr int j = 101;
	// constexpr int j = 100;
	constexpr int j = 50;
	std::conditional <(j > 100), std::map<int, int>, std::conditional<(j > 80), std::string, std::conditional<(j > 40), std::vector<int>, char>::type>::type> ::type obj;
	
	// obj.insert({ 10, 100 });
	// obj = "���Ǵ�ɵ��";
	obj.push_back(10);

}

void TEST::TestFun3()
{
	std::function<void(int)> f = [](int p) {};

	MYSTD::function_call_back<int(int)> f1 = [](int p)
	{
		std::cout << "���Ǵ�ɵ��" << p <<std::endl;
		return p;
	};

	std::cout << "���Ǵ�ɵ��" << f1(10) << std::endl;
}