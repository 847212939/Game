#pragma once

// 实现不同类型之间的运算符重载 这里仅仅实现了std::vector运算符的重载

// 泛化模板类型
template<typename T, typename U, typename V = std::void_t<>>
struct if_can_addition : std::false_type
{
};

// 特化模板类型
template<typename T, typename U>
struct if_can_addition<T, U, std::void_t<decltype(std::declval<T>() + std::declval<U>())>> : std::true_type
{
};

// 泛化模板类型
template<typename T, typename U, bool V = if_can_addition<T, U>::value>
struct addition_result
{
	using type = decltype(std::declval<T>() + std::declval<U>());
};

// 特化模板类型
template<typename T, typename U>
struct addition_result<T, U, false>
{
};

// 返回值类型
template<typename T, typename U>
using addition_result_t = typename addition_result<T, U>::type;

// 重载运算符+
template<typename T, typename U>
std::vector<addition_result_t<T, U>>&& operator+(const std::vector<T>& left, const std::vector<U>& right)
{
	std::vector<addition_result_t<T, U>> t;
	return std::move(t);
}
