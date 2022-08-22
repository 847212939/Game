#include "pch.h"

TCPClient::TCPClient() : 
	CTCPSocketManage(),
	m_pServerTimer(nullptr)
{
	RegisterNetType(TCPClient::SocketCallback, SysMsgCmd::HD_SOCKET_READ);
	RegisterNetType(TCPClient::TimerCallback, SysMsgCmd::HD_TIMER_MESSAGE);
}

bool TCPClient::Init(NetworkCallBackFunc netFunc, TimerCallBackFunc timerFunc)
{
	if (!Start())
	{
		return false;
	}

	m_NetworkCallBackFunc = netFunc;
	m_TimerCallBackFunc = timerFunc;

	if (GetTimerCnt() > 0)
	{
		m_pServerTimer = new CServerTimer[GetTimerCnt()];
		for (int i = 0; i < GetTimerCnt(); i++)
		{
			m_pServerTimer[i].Start();
		}
	}

	GetSockeThreadVec().push_back(new std::thread(&TCPClient::HandlerRecvDataListThread, this));

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
		std::cout << "CDataLine error pDataLine == nullptr" << std::endl;
		run = false;
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

	std::cout << "recv data thread end" << std::endl;

	return;
}

void TCPClient::NotifyAll()
{
	GetConditionVariable().NotifyAll();

	CDataLine* RecvDataLine = GetRecvDataLine();
	CDataLine* SendDataLine = GetSendDataLine();
	if (!RecvDataLine)
	{
		std::cout << "RecvDataLine = null" << std::endl;
		return;
	}
	if (!SendDataLine)
	{
		std::cout << "SendDataLine = null" << std::endl;
		return;
	}
	
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

	std::cout << "There is already a callback for this message. Please check the code cmd=" << (int)cmd << std::endl;
}

bool TCPClient::CallBackFun(SysMsgCmd cmd, void* pDataLineHead)
{
	TypeFunMap::iterator it = m_TypeFunMap.find(cmd);
	if (it == m_TypeFunMap.end())
	{
		std::cout << "No corresponding callback function found cmd=" << (int)cmd << std::endl;
		return false;
	}

	it->second(pDataLineHead);
	return true;
}

void TCPClient::SocketCallback(void* pDataLineHead)
{
	REvent eve;
	//处理数据
	SocketReadLine* pMsg = reinterpret_cast<SocketReadLine*>(pDataLineHead);
	std::string pData = static_cast<char*>(pDataLineHead) + sizeof(SocketReadLine);

	Cos os;
	os	<< pMsg->netMessageHead.uMainID 
		<< pMsg->netMessageHead.uAssistantID
		<< pMsg->netMessageHead.uIdentification 
		<< pMsg->uHandleSize 
		<< pData.c_str();

	memcpy(eve.m_Source, os.str().c_str(), os.str().size());

	m_NetworkCallBackFunc(eve);
}


//设定定时器
bool TCPClient::SetTimer(int uTimerID, unsigned int uElapse, unsigned char timerType/* = SERVERTIMER_TYPE_PERISIST*/)
{
	if (!m_pServerTimer)
	{
		return false;
	}

	m_pServerTimer[(int)uTimerID % GetTimerCnt()].SetTimer((unsigned int)uTimerID, uElapse, timerType);

	return true;
}

//清除定时器
bool TCPClient::KillTimer(int uTimerID)
{
	if (!m_pServerTimer)
	{
		return false;
	}

	m_pServerTimer[(int)uTimerID % GetTimerCnt()].KillTimer((unsigned int)uTimerID);

	return true;
}

void TCPClient::AddTimerCallback(int cmd)
{
	TimerFunMap::iterator it = m_TimerFunMap.find(cmd);
	if (it == m_TimerFunMap.end())
	{
		m_TimerFunMap.insert(std::make_pair(cmd, m_TimerCallBackFunc));
		return;
	}

	std::cout << "There is already a callback for this message. Please check the code cmd=" << cmd << std::endl;
}


bool TCPClient::CallBackFun(int cmd)
{
	TimerFunMap::iterator it = m_TimerFunMap.find(cmd);
	if (it == m_TimerFunMap.end())
	{
		std::cout << "No corresponding callback function found cmd=" << cmd << std::endl;
		return false;
	}

	it->second(cmd);
	return true;
}

void TCPClient::DelTimerCallback(int cmd)
{
	TimerFunMap::iterator it = m_TimerFunMap.find(cmd);
	if (it == m_TimerFunMap.end())
	{
		return;
	}

	m_TimerFunMap.erase(it);
}

void TCPClient::TimerCallback(void* pDataLineHead)
{
	ServerTimerLine* WindowTimer = (ServerTimerLine*)pDataLineHead;
	if (WindowTimer->uMainID == 6)
	{
		CallBackFun((int)WindowTimer->uTimerID);
	}
	else
	{
		std::cout << "Timer message error"  << std::endl;
	}
}


