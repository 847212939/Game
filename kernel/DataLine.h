#pragma 

//���ݶ�����
class CDataLine
{
public:
	CDataLine();
	virtual ~CDataLine();

protected:
	CDataLine(const CDataLine& my);
	CDataLine& operator=(const CDataLine& my);

public:
	//������������
	bool CleanData();
	//������Ϣ����
	virtual unsigned int AddData(void* pData, unsigned int uDataSize, unsigned int uDataKind);
	//��ȡ��Ϣ����
	virtual unsigned int GetData(void** pDataBuffer, bool& run, unsigned int& uDataKind);
	//��ȡ���д�С
	size_t GetDataCount() { return m_dataListSize; };
	ConditionVariable& GetConditionVariable();

private:
	std::atomic<unsigned int>	m_dataListSize;
	std::list <ListItemData*>	m_dataList;
	ConditionVariable			m_cond;
};