#pragma once

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
	function_hander_child(FT&& f) : m_function(std::forward<FT>(f))
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
		m_pHander = new function_hander_child<FT, RT, Args...>(std::forward<FT>(func));
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
