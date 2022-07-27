#pragma 

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
	virtual unsigned int GetData(void** pDataBuffer, bool& run, unsigned int& uDataKind);
	//获取队列大小
	size_t GetDataCount() { return m_dataListSize; };
	ConditionVariable& GetConditionVariable();

private:
	std::atomic<unsigned int>	m_dataListSize;
	std::list <ListItemData*>	m_dataList;
	ConditionVariable			m_cond;
};