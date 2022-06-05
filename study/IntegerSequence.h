#pragma once

// 理解递归调用的方法

template<typename T, int pushs, int... ints>
struct IntegerSequencePushBack;

template<typename T, int pushs, int... ints>
struct IntegerSequencePushBack<std::integer_sequence<T, ints...>, pushs>
{
	using type = std::integer_sequence<T, ints..., pushs>;
};

// 递归
// 泛化版本
template<typename T, int N>
struct IntegerSequence
{
	using type = typename IntegerSequencePushBack<typename IntegerSequence<T, N - 1>::type, N - 1>::type;
};

// 特化版本
template<typename T>
struct IntegerSequence<T, 1>
{
	using type = std::integer_sequence<T, 0>;
};