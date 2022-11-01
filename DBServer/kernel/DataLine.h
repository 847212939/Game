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
	// ������Ϣ����
	unsigned int AddData(void* pData, unsigned int uDataSize, SysMsgCmd uDataKind = SysMsgCmd::HD_SYS_MSG_BEGIN);
	// ��ȡ��Ϣ����
	unsigned int GetData(ListItemData** pDataBuffer, bool& run, unsigned int& uDataKind);
	std::condition_variable& GetConditionVariable();

private:
	std::list <ListItemData*>	m_dataList;
	std::mutex					m_mutex;
	std::condition_variable		m_cond;
};