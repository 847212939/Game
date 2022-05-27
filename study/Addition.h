#pragma once

// ʵ�ֲ�ͬ����֮������������ �������ʵ����std::vector�����������

// ����ģ������
template<typename T, typename U, typename V = std::void_t<>>
struct if_can_addition : std::false_type
{
};

// �ػ�ģ������
template<typename T, typename U>
struct if_can_addition<T, U, std::void_t<decltype(std::declval<T>() + std::declval<U>())>> : std::true_type
{
};

// ����ģ������
template<typename T, typename U, bool V = if_can_addition<T, U>::value>
struct addition_result
{
	using type = decltype(std::declval<T>() + std::declval<U>());
};

// �ػ�ģ������
template<typename T, typename U>
struct addition_result<T, U, false>
{
};

// ����ֵ����
template<typename T, typename U>
using addition_result_t = typename addition_result<T, U>::type;

// ���������+
template<typename T, typename U>
std::vector<addition_result_t<T, U>>&& operator+(const std::vector<T>& left, const std::vector<U>& right)
{
	std::vector<addition_result_t<T, U>> t;
	return std::move(t);
}
