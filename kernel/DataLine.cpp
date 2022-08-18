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

unsigned int CDataLine::AddData(void* pData, unsigned int uDataSize, SysMsgCmd uDataKind)
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
	ListItemData* pListItem = new ListItemData;

	pListItem->pData = nullptr;
	pListItem->stDataHead.uSize = uDataSize;
	pListItem->stDataHead.uDataKind = (unsigned int)uDataKind;

	unsigned int size = pListItem->stDataHead.uSize + 1;
	pListItem->pData = new unsigned char[size];
	pListItem->pData[pListItem->stDataHead.uSize] = 0;

	memcpy(pListItem->pData, pData, uDataSize);

	m_cond.GetMutex().lock();
	m_dataList.push_back(pListItem);
	m_dataListSize++;
	m_cond.GetMutex().unlock();

	m_cond.NotifyOne();

	return pListItem->stDataHead.uSize;
}

unsigned int CDataLine::GetData(void** pDataBuffer, bool& run, unsigned int& uDataKind)
{
	*pDataBuffer = nullptr;

	std::unique_lock<std::mutex> uniqLock(m_cond.GetMutex());
	m_cond.Wait(uniqLock, [this, &run] { if (this->m_dataListSize > 0 || !run) { return true; } return false; });

	if (m_dataListSize <= 0)
	{
		uniqLock.unlock();
		return 0;
	}

	ListItemData* pListItem = m_dataList.front();
	m_dataList.pop_front();

	m_dataListSize--;

	uniqLock.unlock();

	uDataKind = pListItem->stDataHead.uDataKind;
	unsigned int uDataSize = pListItem->stDataHead.uSize;

	//投递数据，外部一定要释放内存，否则内存泄漏
	*pDataBuffer = reinterpret_cast<DataLineHead*>(pListItem->pData);

	//删除队列中的数据
	SafeDelete(pListItem);

	return uDataSize;
}

bool CDataLine::SwapDataList(std::list <ListItemData*>& dataList, bool& run)
{
	dataList.clear();
	std::list <ListItemData*>& mDataList = m_dataList;
	std::unique_lock<std::mutex> uniqLock(m_cond.GetMutex());
	m_cond.Wait(uniqLock, [&mDataList, &run]
		{
			if (mDataList.size() > 0 || !run)
			{
				return true;
			}
			return false;
		});
	if (m_dataListSize <= 0)
	{
		uniqLock.unlock();
		return false;
	}

	dataList.swap(m_dataList);
	m_dataListSize = 0;
	uniqLock.unlock();

	return true;
}

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

size_t CDataLine::GetDataCount()
{
	return m_dataListSize;
}
