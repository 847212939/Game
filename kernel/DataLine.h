#pragma 

class CDataLine
{
public:
	CDataLine();
	virtual ~CDataLine();

protected:
	CDataLine(const CDataLine& my);
	CDataLine& operator=(const CDataLine& my);

public:
	// ������������
	bool CleanData();
	// ��ȡ���д�С
	size_t GetDataCount();
	// ��ȡ��������
	ConditionVariable& GetConditionVariable();
	// �������ݱ����������
	bool SwapDataList(std::list<ListItemData*>& dataList, bool& run);
	// ������Ϣ����
	unsigned int AddData(void* pData, unsigned int uDataSize, unsigned int uDataKind);
	// ��ȡ��Ϣ����
	unsigned int GetData(void** pDataBuffer, bool& run, unsigned int& uDataKind);

private:
	unsigned int				m_dataListSize;
	std::list <ListItemData*>	m_dataList;
	ConditionVariable			m_cond;
};