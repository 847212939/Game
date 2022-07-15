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
		COUT_LOG(LOG_CERROR, "��������(%d)", MAX_DATALINE_LEN);
		return 0;
	}
	ListItemData* pListItem = new ListItemData;			//����һ��������

	pListItem->pData = nullptr;							//����Ϊ0���Ա�֤����������
	pListItem->stDataHead.uSize = uDataSize;			//���ݴ�С
	pListItem->stDataHead.uDataKind = uDataKind;		//��������

	unsigned int size = pListItem->stDataHead.uSize + 1;
	pListItem->pData = new unsigned char[size];
	pListItem->pData[pListItem->stDataHead.uSize] = 0;

	memcpy(pListItem->pData, pData, uDataSize);

	// ����
	m_cond.GetMutex().lock();
	m_dataList.push_back(pListItem);								//�ӵ�����β��
	m_dataListSize++;
	m_cond.GetMutex().unlock();

	// ֪ͨ��������
	m_cond.NotifyOne();

	return pListItem->stDataHead.uSize;		//���ش�С
}

unsigned int CDataLine::GetData(void** pDataBuffer, bool& run)
{
	*pDataBuffer = nullptr;

	//�������״̬
	std::unique_lock<std::mutex> uniqLock(m_cond.GetMutex());
	m_cond.Wait(uniqLock, [this, &run] { if (this->m_dataListSize > 0 || !run) { return true; } return false; });

	//��������ǿյģ�ֱ�ӷ���
	if (m_dataListSize <= 0)
	{
		uniqLock.unlock();
		return 0;
	}

	//ȡ����
	ListItemData* pListItem = m_dataList.front();
	m_dataList.pop_front();

	m_dataListSize--;

	uniqLock.unlock();

	unsigned int uDataSize = pListItem->stDataHead.uSize;

	//Ͷ�����ݣ��ⲿһ��Ҫ�ͷ��ڴ棬�����ڴ�й©
	*pDataBuffer = reinterpret_cast<DataLineHead*>(pListItem->pData);

	//ɾ�������е�����
	delete pListItem;

	return uDataSize;
}

//������������
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