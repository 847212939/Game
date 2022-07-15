#include "../Game/stdafx.h"

CDataLine::CDataLine() : m_dataListSize(0)
{
	m_dataList.clear();
}

CDataLine::~CDataLine()
{
	ListItemData* pListItem = nullptr;

	while (m_dataList.size() > 0)
	{
		pListItem = m_dataList.front();
		m_dataList.pop_front();
		if (pListItem)
		{
			SafeDeleteArray(pListItem->pData);
		}
		SafeDelete(pListItem);
	}
}

unsigned int CDataLine::AddData(void* pData, unsigned int uDataSize, unsigned int uDataKind)
{
	if (!pData || uDataSize == 0)
	{
		return 0;
	}
	if (m_dataListSize > MAX_DATALINE_LEN)
	{
		COUT_LOG(LOG_CERROR, "队列已满(%d)", MAX_DATALINE_LEN);
		return 0;
	}
	ListItemData* pListItem = new ListItemData;			//创建一个队列项

	pListItem->pData = nullptr;							//先设为0，以保证后续不出错
	pListItem->stDataHead.uSize = uDataSize;			//数据大小
	pListItem->stDataHead.uDataKind = uDataKind;		//数据类型

	unsigned int size = pListItem->stDataHead.uSize + 1;
	pListItem->pData = new unsigned char[size];
	pListItem->pData[pListItem->stDataHead.uSize] = 0;

	memcpy(pListItem->pData, pData, uDataSize);

	// 加锁
	m_cond.GetMutex().lock();
	m_dataList.push_back(pListItem);								//加到队列尾部
	m_dataListSize++;
	m_cond.GetMutex().unlock();

	// 通知条件变量
	m_cond.NotifyOne();

	return pListItem->stDataHead.uSize;		//返回大小
}

unsigned int CDataLine::GetData(void** pDataBuffer, bool& run)
{
	*pDataBuffer = nullptr;

	//进入挂起状态
	std::unique_lock<std::mutex> uniqLock(m_cond.GetMutex());
	m_cond.Wait(uniqLock, [this, &run] { if (this->m_dataListSize > 0 || !run) { return true; } return false; });

	//如果队列是空的，直接返回
	if (m_dataListSize <= 0)
	{
		uniqLock.unlock();
		return 0;
	}

	//取数据
	ListItemData* pListItem = m_dataList.front();
	m_dataList.pop_front();

	m_dataListSize--;

	uniqLock.unlock();

	unsigned int uDataSize = pListItem->stDataHead.uSize;

	//投递数据，外部一定要释放内存，否则内存泄漏
	*pDataBuffer = reinterpret_cast<DataLineHead*>(pListItem->pData);

	//删除队列中的数据
	delete pListItem;

	return uDataSize;
}

//清理所有数据
bool CDataLine::CleanData()
{
	ListItemData* pListItem = nullptr;
	std::lock_guard<std::mutex> guard(m_cond.GetMutex());

	while (m_dataList.size() > 0)
	{
		pListItem = m_dataList.front();
		m_dataList.pop_front();
		if (pListItem)
		{
			SafeDeleteArray(pListItem->pData);

		}
		SafeDelete(pListItem);
	}

	m_dataListSize = 0;

	return true;
}

ConditionVariable& CDataLine::GetConditionVariable()
{
	return m_cond;
}