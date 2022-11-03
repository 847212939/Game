#include "../stdafx.h"

CDataLine::CDataLine()
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
	if (m_dataList.size() > MAX_DATALINE_LEN)
	{
		Log(CERR, "¶ÓÁÐÒÑÂú(%d)", MAX_DATALINE_LEN);
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

	m_mutex.lock();
	m_dataList.push_back(pListItem);
	m_mutex.unlock();

	m_cond.notify_one();

	return pListItem->stDataHead.uSize;
}

unsigned int CDataLine::GetData(ListItemData** pDataBuffer, bool& run, unsigned int& uDataKind)
{
	*pDataBuffer = nullptr;

	std::unique_lock<std::mutex> uniqLock(m_mutex);
	m_cond.wait(uniqLock, [this, &run] {return this->m_dataList.size() > 0 || !run; });

	*pDataBuffer = m_dataList.front();
	m_dataList.pop_front();
	uniqLock.unlock();

	if (!(*pDataBuffer))
	{
		return 0;
	}
	uDataKind = (*pDataBuffer)->stDataHead.uDataKind;
	unsigned int uDataSize = (*pDataBuffer)->stDataHead.uSize;

	return uDataSize;
}

bool CDataLine::CleanData()
{
	ListItemData* pListItem = nullptr;
	std::lock_guard<std::mutex> guard(m_mutex);

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
	return true;
}

std::condition_variable& CDataLine::GetConditionVariable()
{
	return m_cond;
}
