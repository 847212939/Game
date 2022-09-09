#include "pch.h"

TCPClient::TCPClient() : 
	CTCPSocketManage(),
	m_pServerTimer(nullptr)
{
	RegisterNetType(TCPClient::SocketCallback, SysMsgCmd::HD_SOCKET_READ);
	RegisterNetType(TCPClient::TimerCallback, SysMsgCmd::HD_TIMER_MESSAGE);
	RegisterNetType(TCPClient::CloseSocketCallback, SysMsgCmd::HD_SOCKET_CLOSE);
}

bool TCPClient::Init(NetworkCallBackFunc netFunc, TimerCallBackFunc timerFunc, CloseCallBackFunc closeFunc)
{
	if (!Start())
	{
		return false;
	}
	if (!m_NetworkCallBackFunc)
	{
		m_NetworkCallBackFunc = netFunc;
	}
	if (!m_TimerCallBackFunc)
	{
		m_TimerCallBackFunc = timerFunc;
	}
	if (!m_CloseCallBackFunc)
	{
		m_CloseCallBackFunc = closeFunc;
	}
	if (GetTimerCnt() > 0)
	{
		if (!m_pServerTimer)
		{
			m_pServerTimer = new CServerTimer[GetTimerCnt()];
		}
		for (int i = 0; i < GetTimerCnt(); i++)
		{
			m_pServerTimer[i].Start();
		}
	}

	std::thread handlerRecvDataListThread(&TCPClient::HandlerRecvDataListThread, this);
	handlerRecvDataListThread.detach();

	return true;
}

TCPClient::~TCPClient()
{
	if (m_pServerTimer)
	{
		SafeDeleteArray(m_pServerTimer);
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
		run = false;
		return;
	}
	while (true)
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

	return;
}

void TCPClient::NotifyAll()
{
	Stop();

	for (int i = 0; i < GetTimerCnt(); i++)
	{
		m_pServerTimer[i].SetTimerRun(false);
	}
}

void TCPClient::AddNetTypeCallback(SysMsgCmd cmd, std::function<void(void* pDataLineHead)>&& fun)
{
	TypeFunMap::iterator it = m_TypeFunMap.find(cmd);
	if (it == m_TypeFunMap.end())
	{
		m_TypeFunMap.insert(std::make_pair(cmd, fun));
		return;
	}
}

bool TCPClient::CallBackFun(SysMsgCmd cmd, void* pDataLineHead)
{
	TypeFunMap::iterator it = m_TypeFunMap.find(cmd);
	if (it == m_TypeFunMap.end())
	{
		return false;
	}

	it->second(pDataLineHead);
	return true;
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
}


bool TCPClient::CallBackFun(int cmd)
{
	TimerFunMap::iterator it = m_TimerFunMap.find(cmd);
	if (it == m_TimerFunMap.end())
	{
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

void TCPClient::SocketCallback(void* pDataLineHead)
{
	static REvent eve;

	SocketReadLine* pMsg = reinterpret_cast<SocketReadLine*>(pDataLineHead);
	std::string pData = static_cast<char*>(pDataLineHead) + sizeof(SocketReadLine);

	Cos os;
	os << pMsg->netMessageHead.uMainID
		<< pMsg->netMessageHead.uAssistantID
		<< pMsg->netMessageHead.uIdentification
		<< pMsg->uHandleSize
		<< pData.c_str();

	memcpy(eve.m_Source, os.str().c_str(), os.str().size());

	m_NetworkCallBackFunc(eve);
}

void TCPClient::TimerCallback(void* pDataLineHead)
{
	ServerTimerLine* WindowTimer = (ServerTimerLine*)pDataLineHead;
	if (WindowTimer->uMainID == MsgCmd_Timer)
	{
		CallBackFun((int)WindowTimer->uTimerID);
	}
}

void TCPClient::CloseSocketCallback(void* pDataLineHead)
{
	NotifyAll();
	m_CloseCallBackFunc();

	const SockFd& fd = GetSocket();

#if defined(_WIN32)
	closesocket(fd);
#elif defined(_WIN64)
#elif defined(__linux__)
	close(fd);
#elif defined(__unix__)
#elif defined(__ANDROID__)
#elif defined(__APPLE__)
#endif
}
