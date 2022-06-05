#include "../Game/stdafx.h"

void TEST::TestFun()
{
	// 模板混入实现多个基类的继承
	RoleNpc roleNpc;
	roleNpc.m_speack = "你是傻逼";

	RoleNpcFamily roleNpcFamily;
	roleNpcFamily.m_familyVec.push_back(roleNpc);

}

void TEST::TestFun1()
{
	std::vector<int> a;
	std::vector<double> b;

	// 实现std::vector加法
	auto c = a + b;
}

void TEST::TestFun2()
{
	std::conditional<true, int, std::string>::type c = 10;
	std::conditional<false, int, std::string>::type s = "123456";

	// 编译期间实现分支逻辑
	// constexpr int j = 101;
	// constexpr int j = 100;
	constexpr int j = 50;
	std::conditional <(j > 100), std::map<int, int>, std::conditional<(j > 80), std::string, std::conditional<(j > 40), std::vector<int>, char>::type>::type> ::type obj;
	
	// obj.insert({ 10, 100 });
	// obj = "你是大傻逼";
	obj.push_back(10);

}

void TEST::TestFun3()
{
	std::function<void(int)> f = [](int p) {};

	function_call_back<int(int)> f1 = [](int p)
	{
		std::cout << "你是大傻逼" << p <<std::endl;
		return p;
	};

	std::cout << "你是大傻逼" << f1(10) << std::endl;
}

void TEST::TestFun4()
{
	int arr[8][8][8];
	std::cout << typeid(decltype(arr)).name() << std::endl;

	// 把一个数组中的数组类型部分移除掉，只保留元素类型。
	// 采用递归实现手法
	std::remove_all_extents<decltype(arr)>::type a = 10;

	std::cout << typeid(decltype(a)).name() << std::endl;
}

// std::integer_sequence
void TEST::TestFun5()		
{
	IntegerSequence<int, 5>::type tmpobj5;
	std::cout << "tmpobj5的类型为：" << typeid(decltype(tmpobj5)).name() << std::endl;
}