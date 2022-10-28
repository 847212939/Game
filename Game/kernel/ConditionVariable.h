#pragma once

class ConditionVariable
{
public:
	ConditionVariable() {}
	virtual ~ConditionVariable() {}

protected:
	// ���ÿ������캯��
	ConditionVariable(const ConditionVariable& my);
	ConditionVariable& operator=(const ConditionVariable& my);

public:
	/*
	* @brief ��û�����
	*/
	std::mutex* GetMutex()
	{
		return &m_mutex;
	}
	/*
	* @brief ֪ͨһ���������߳�
	*/
	void NotifyOne()
	{
		m_cond.notify_one();
	}
	/*
	* @brief ֪ͨ�����������߳�
	*/
	void NotifyAll()
	{
		m_cond.notify_all();
	}
	/*
	* @brief ������֪ͨ����
	* @param func �º�������lambda ����ֵΪtrue��ʾ�ȴ��ɹ�����
	* @DATALIST һ��λ��Ҫ���������ݶ���
	*/
	void Wait(std::unique_lock<std::mutex>& uniqLock, const std::function<bool()>& func)
	{
		m_cond.wait(uniqLock, func);
	}
	/*
	* @brief ������֪ͨ���ѻ��߶೤ʱ��󱻻���
	* @@param milSec �ȴ����ٺ���
	*/
	void Wait_for(std::unique_lock<std::mutex>& uniqLock, std::chrono::milliseconds& milSec)
	{
		m_cond.wait_for(uniqLock, milSec);
	}
private:
	std::mutex m_mutex;
	std::condition_variable m_cond;
};