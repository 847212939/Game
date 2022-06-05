#pragma once

// ���ݹ���õķ���

template<typename T, int pushs, int... ints>
struct IntegerSequencePushBack;

template<typename T, int pushs, int... ints>
struct IntegerSequencePushBack<std::integer_sequence<T, ints...>, pushs>
{
	using type = std::integer_sequence<T, ints..., pushs>;
};

// �ݹ�
// �����汾
template<typename T, int N>
struct IntegerSequence
{
	using type = typename IntegerSequencePushBack<typename IntegerSequence<T, N - 1>::type, N - 1>::type;
};

// �ػ��汾
template<typename T>
struct IntegerSequence<T, 1>
{
	using type = std::integer_sequence<T, 0>;
};