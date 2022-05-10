#pragma once
#include <list>
#include <atomic>
#include "ConditionVariable.h"
#include "Internal.h"

namespace DRMU
{
	// 消息队列最大长度
	const unsigned int MAX_DATALINE_LEN = 320000;

	/*
	Struct		:ListItemData
	Memo		:链表项数据结构
	Parameter	:
		stDataHead	:数据包头
		pData		:每个链表项数据的指针，使用用new方式申请的内存，注意，在出队列时，要显式的delete 该内存
	*/
	struct ListItemData
	{
		DataLineHead stDataHead;
		unsigned char* pData;
		ListItemData() : pData(nullptr) {}
	};

	//数据队列类
	class CDataLine
	{
	public:
		CDataLine();
		virtual ~CDataLine();

	public:
		//清理所有数据
		bool CleanData();
		//加入消息队列
		virtual unsigned int AddData(void* pData, unsigned int uDataSize, unsigned int uDataKind);
		//提取消息数据
		virtual unsigned int GetData(void** pDataBuffer);
		//获取队列大小
		size_t GetDataCount() { return m_dataListSize; };

	private:
		std::atomic<unsigned int>	m_dataListSize;
		std::list <ListItemData*>	m_dataList;
		ConditionVariable		m_cond;

	};
}
