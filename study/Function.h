#pragma once

namespace MYSTD
{
	// STRUCT TEMPLATE integral_constant
	template <class T, T _Val>
	struct integral_constant_my 
	{
		static constexpr T value = _Val;

		using value_type = T;
		using type = integral_constant_my;

		constexpr operator value_type() const noexcept 
		{
			return value;
		}

		constexpr value_type operator()() const noexcept 
		{
			return value;
		}
	};

	// ALIAS TEMPLATE bool_constant
	template <bool _Val>
	using bool_constant_my = integral_constant_my<bool, _Val>;

	// STRUCT TEMPLATE is_lvalue_reference
	template <class>
	constexpr bool is_lvalue_reference_v_my = false; // determine whether type argument is an lvalue reference

	template <class T>
	constexpr bool is_lvalue_reference_v_my<T&> = true;

	template <class T>
	struct is_lvalue_reference_my : bool_constant_my<is_lvalue_reference_v_my<T>> {};

	// STRUCT TEMPLATE remove_reference
	template <class T>
	struct remove_reference_my 
	{
		using type = T;
		using _Const_thru_ref_type_my = const T;
	};

	template <class T>
	struct remove_reference_my<T&> 
	{
		using type = T;
		using _Const_thru_ref_type_my = const T&;
	};

	template <class T>
	struct remove_reference_my<T&&> 
	{
		using type = T;
		using _Const_thru_ref_type_my = const T&&;
	};

	template <class T>
	using remove_reference_my_t = typename remove_reference_my<T>::type;

	// FUNCTION TEMPLATE forward
	template <class T>
	constexpr T&& my_forward(remove_reference_my_t<T>& _Arg) noexcept 
	{ // forward an lvalue as either an lvalue or an rvalue
		return static_cast<T&&>(_Arg);
	}

	template <class T>
	constexpr T&& my_forward(remove_reference_my_t<T>&& _Arg) noexcept 
	{ // forward an rvalue as an rvalue
		static_assert(!is_lvalue_reference_v_my<T>, "bad forward call");
		return static_cast<T&&>(_Arg);
	}

	template<typename RT, typename... Args>
	class function_hander
	{
	public:
		virtual RT call_back(Args... args) = 0;
	};

	template<typename FT, typename RT, typename... Args>
	class function_hander_child : public function_hander<RT, Args...>
	{
	public:
		function_hander_child(FT&& f) : m_function(my_forward<FT>(f))
		{
		}

		virtual RT call_back(Args... args)
		{
			return m_function(args...);
		}

	private:
		FT m_function;
	};

	template<typename RT, typename... Args>
	class function_call_back
	{
	};

	template<typename RT, typename... Args>
	class function_call_back<RT(Args...)>
	{
	public:
		// 隐式类型转换
		// 万能引用
		// 完美转发
		template<typename FT>
		function_call_back(FT&& func)
		{
			m_pHander = new function_hander_child<FT, RT, Args...>(my_forward<FT>(func));
		}

		// 可调用对象
		RT operator()(Args... args)
		{
			return m_pHander->call_back(args...);
		}

		~function_call_back()
		{
			if (m_pHander)
			{
				delete m_pHander;
			}
		}

	private:
		function_hander<RT, Args...>* m_pHander;
	};

}

