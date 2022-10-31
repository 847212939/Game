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
	// ������Ϣ����
	unsigned int AddData(void* pData, unsigned int uDataSize, SysMsgCmd uDataKind = SysMsgCmd::HD_SYS_MSG_BEGIN);
	// ��ȡ��Ϣ����
	unsigned int GetData(ListItemData** pDataBuffer, bool& run, unsigned int& uDataKind);

public:
	std::condition_variable& GetConditionVariable()
	{
		return m_cond;
	}

private:
	unsigned int				m_dataListSize;
	std::list <ListItemData*>	m_dataList;
	std::mutex					m_mutex;
	std::condition_variable		m_cond;
};