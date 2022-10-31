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
	// 清理所有数据
	bool CleanData();
	// 获取队列大小
	size_t GetDataCount();
	// 获取条件变量
	ConditionVariable& GetConditionVariable();
	// 加入消息队列
	unsigned int AddData(void* pData, unsigned int uDataSize, SysMsgCmd uDataKind = SysMsgCmd::HD_SYS_MSG_BEGIN);
	// 提取消息数据
	unsigned int GetData(ListItemData** pDataBuffer, bool& run, unsigned int& uDataKind);

private:
	unsigned int				m_dataListSize;
	std::list <ListItemData*>	m_dataList;
	ConditionVariable			m_cond;
};