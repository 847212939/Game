#pragma once
#include <list>
#include <atomic>
#include "ConditionVariable.h"
#include "Internal.h"

namespace DRMU
{
	// ��Ϣ������󳤶�
	const unsigned int MAX_DATALINE_LEN = 320000;

	/*
	Struct		:ListItemData
	Memo		:���������ݽṹ
	Parameter	:
		stDataHead	:���ݰ�ͷ
		pData		:ÿ�����������ݵ�ָ�룬ʹ����new��ʽ������ڴ棬ע�⣬�ڳ�����ʱ��Ҫ��ʽ��delete ���ڴ�
	*/
	struct ListItemData
	{
		DataLineHead stDataHead;
		unsigned char* pData;
		ListItemData() : pData(nullptr) {}
	};

	//���ݶ�����
	class CDataLine
	{
	public:
		CDataLine();
		virtual ~CDataLine();

	public:
		//������������
		bool CleanData();
		//������Ϣ����
		virtual unsigned int AddData(void* pData, unsigned int uDataSize, unsigned int uDataKind);
		//��ȡ��Ϣ����
		virtual unsigned int GetData(void** pDataBuffer);
		//��ȡ���д�С
		size_t GetDataCount() { return m_dataListSize; };

	private:
		std::atomic<unsigned int>	m_dataListSize;
		std::list <ListItemData*>	m_dataList;
		ConditionVariable		m_cond;

	};
}
