#include "pch.h"

TCPClient::TCPClient() : CTCPSocketManage()
{
	RegisterNetType(TCPClient::SocketCallback, SysMsgCmd::HD_SOCKET_READ);
}

bool TCPClient::Init(ServiceType serverType)
{
	const CLogicCfg& logicCfg = BaseCfgMgr.GetLogicCfg();
	int maxSocketCnt = BaseCfgMgr.GetMaxSocketCnt();

	if (!Start(serverType))
	{
		return false;
	}

	GetSockeThreadVec().push_back(new std::thread(&TCPClient::HandlerRecvDataListThread, this));

	COUT_LOG(LOG_CINFO, "Server initialization succeeded");
	return true;
}

TCPClient::~TCPClient()
{
	std::vector<std::thread*>& threadVec = GetSockeThreadVec();
	while (!threadVec.empty())
	{
		std::vector<std::thread*>::iterator it = threadVec.begin();
		if (*it)
		{
			(*it)->join();
			SafeDelete(*it);
		}

		threadVec.erase(it);
	}
}

void TCPClient::HandleRecvData(ListItemData* pListItem)
{
	if (!pListItem)
	{
		return;
	}
	if (pListItem->stDataHead.uSize == 0 ||
		pListItem->pData == nullptr ||
		pListItem->stDataHead.uDataKind <= 0)
	{
		return;
	}
	CallBackFun((SysMsgCmd)pListItem->stDataHead.uDataKind, (void*)pListItem->pData);

	SafeDeleteArray(pListItem->pData);
	SafeDelete(pListItem);
}

void TCPClient::HandlerRecvDataListThread()
{
	bool& run = GetRuninged();
	std::list <ListItemData*> dataList;
	CDataLine* pDataLine = GetRecvDataLine();
	if (!pDataLine)
	{
		COUT_LOG(LOG_CERROR, "CDataLine error pDataLine == nullptr");
		return;
	}
	while (run)
	{
		if (!pDataLine->SwapDataList(dataList, run))
		{
			continue;
		}
		while (!dataList.empty())
		{
			HandleRecvData(dataList.front());
			dataList.pop_front();
		}
	}

	COUT_LOG(LOG_CINFO, "recv data thread end");

	return;
}

void TCPClient::NotifyAll()
{
	GetConditionVariable().NotifyAll();

	CDataLine* RecvDataLine = GetRecvDataLine();
	CDataLine* SendDataLine = GetSendDataLine();
	if (!RecvDataLine)
	{
		COUT_LOG(LOG_CERROR, "RecvDataLine = null");
		return;
	}
	if (!SendDataLine)
	{
		COUT_LOG(LOG_CERROR, "SendDataLine = null");
		return;
	}
	
	int timerCnt = BaseCfgMgr.GetTimerCnt();

	RecvDataLine->GetConditionVariable().NotifyAll();
	SendDataLine->GetConditionVariable().NotifyAll();
}

void TCPClient::AddNetTypeCallback(SysMsgCmd cmd, std::function<void(void* pDataLineHead)>&& fun)
{
	TypeFunMap::iterator it = m_TypeFunMap.find(cmd);
	if (it == m_TypeFunMap.end())
	{
		m_TypeFunMap.insert(std::make_pair(cmd, fun));
		return;
	}

	COUT_LOG(LOG_CINFO, "There is already a callback for this message. Please check the code cmd = %d", cmd);
}

bool TCPClient::CallBackFun(SysMsgCmd cmd, void* pDataLineHead)
{
	TypeFunMap::iterator it = m_TypeFunMap.find(cmd);
	if (it == m_TypeFunMap.end())
	{
		COUT_LOG(LOG_CERROR, "No corresponding callback function found cmd = %d", cmd);
		return false;
	}

	it->second(pDataLineHead);
	return true;
}

void TCPClient::SocketCallback(void* pDataLineHead)
{
	//处理数据
	SocketReadLine* pMsg = reinterpret_cast<SocketReadLine*>(pDataLineHead);
	void* pData = static_cast<char*>(pDataLineHead) + sizeof(SocketReadLine);

	unsigned int size = pMsg->uHandleSize;

	
}
