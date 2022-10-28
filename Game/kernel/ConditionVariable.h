#pragma once

class ConditionVariable
{
public:
	ConditionVariable() {}
	virtual ~ConditionVariable() {}

protected:
	// 禁用拷贝构造函数
	ConditionVariable(const ConditionVariable& my);
	ConditionVariable& operator=(const ConditionVariable& my);

public:
	/*
	* @brief 获得互斥锁
	*/
	std::mutex* GetMutex()
	{
		return &m_mutex;
	}
	/*
	* @brief 通知一个阻塞的线程
	*/
	void NotifyOne()
	{
		m_cond.notify_one();
	}
	/*
	* @brief 通知所有阻塞的线程
	*/
	void NotifyAll()
	{
		m_cond.notify_all();
	}
	/*
	* @brief 阻塞等通知唤醒
	* @param func 仿函数或者lambda 返回值为true表示等待成功解锁
	* @DATALIST 一般位需要加锁的数据队列
	*/
	void Wait(std::unique_lock<std::mutex>& uniqLock, const std::function<bool()>& func)
	{
		m_cond.wait(uniqLock, func);
	}
	/*
	* @brief 阻塞等通知唤醒或者多长时间后被唤醒
	* @@param milSec 等待多少毫秒
	*/
	void Wait_for(std::unique_lock<std::mutex>& uniqLock, std::chrono::milliseconds& milSec)
	{
		m_cond.wait_for(uniqLock, milSec);
	}
private:
	std::mutex m_mutex;
	std::condition_variable m_cond;
};