#include "pch.h"

TCPClient::TCPClient() : 
	CTCPSocketManage(), 
	m_PlayerPrepClient(new PlayerPrepClient),
	m_NetworkCallBackFunc(nullptr),
	m_TimerCallBackFunc(nullptr),
	m_CloseCallBackFunc(nullptr)
{
	RegisterNetType(TCPClient::SocketCallback, SysMsgCmd::HD_SOCKET_READ);
	RegisterNetType(TCPClient::TimerCallback, SysMsgCmd::HD_TIMER_MESSAGE);
	RegisterNetType(TCPClient::CloseSocketCallback, SysMsgCmd::HD_SOCKET_CLOSE);
}
TCPClient::~TCPClient()
{
	if (m_PlayerPrepClient)
	{
		SafeDelete(m_PlayerPrepClient);
	}
}

bool TCPClient::InitNetwork(ClientInfo clientInfo)
{
	GetClientInfo() = clientInfo;

	return true;
}

bool TCPClient::InitCallBack(NetworkCallBackFunc netFunc,
	TimerCallBackFunc timerFunc, CloseCallBackFunc closeFunc)
{
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

	return true;
}

bool TCPClient::Init(ServiceType serverType)
{
	if (!CTCPSocketManage::Init(1024, serverType))
	{
		return false;
	}
	if (!Start())
	{
		return false;
	}

	m_PlayerPrepClient->Init();

	GetSockeThreadVec().push_back(new std::thread(&TCPClient::HandlerRecvDataListThread, this));

	Log(CINF, "Server initialization succeeded");
	return true;
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
	CDataLine* pDataLine = GetRecvDataLine();
	if (!pDataLine)
	{
		Log(CERR, "CDataLine error pDataLine == nullptr");
		return;
	}
	ListItemData* pListItem = NULL;
	unsigned int uDataKind = 0;
	while (run)
	{
		unsigned int bytes = pDataLine->GetData(&pListItem, run, uDataKind);
		if (bytes == 0 || pListItem == NULL)
		{
			continue;
		}
		HandleRecvData(pListItem);
	}

	Log(CINF, "recv data thread end");

	return;
}
TimerCallBackFunc TCPClient::GetTimerCallBackFunc()
{
	return m_TimerCallBackFunc;
}
PlayerPrepClient* TCPClient::GetPlayerPrepClient()
{
	return m_PlayerPrepClient;
}

void TCPClient::NotifyAll()
{
	CDataLine* RecvDataLine = GetRecvDataLine();
	CDataLine* SendDataLine = GetSendDataLine();
	if (!RecvDataLine)
	{
		Log(CERR, "RecvDataLine = null");
		return;
	}
	if (!SendDataLine)
	{
		Log(CERR, "SendDataLine = null");
		return;
	}
	
	RecvDataLine->GetConditionVariable().notify_all();
	SendDataLine->GetConditionVariable().notify_all();

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

void TCPClient::AddNetTypeCallback(SysMsgCmd cmd, std::function<void(void* pDataLineHead)>&& fun)
{
	MapTypeFunc::iterator it = m_TypeFunMap.find(cmd);
	if (it == m_TypeFunMap.end())
	{
		m_TypeFunMap.insert(std::make_pair(cmd, fun));
		return;
	}

	Log(CINF, "There is already a callback for this message. Please check the code cmd = %d", cmd);
}
bool TCPClient::CallBackFun(SysMsgCmd cmd, void* pDataLineHead)
{
	MapTypeFunc::iterator it = m_TypeFunMap.find(cmd);
	if (it == m_TypeFunMap.end())
	{
		Log(CERR, "No corresponding callback function found cmd = %d", cmd);
		return false;
	}

	it->second(pDataLineHead);
	return true;
}

void TCPClient::TimerCallback(void* pDataLineHead)
{
	ServerTimerLine* WindowTimer = (ServerTimerLine*)pDataLineHead;
	if (WindowTimer->uMainID == (unsigned int)MsgCmd::MsgCmd_Timer)
	{
		m_PlayerPrepClient->CallBackFun((TimerCmd)WindowTimer->uTimerID);
	}
	else
	{
		Log(CERR, "Timer message error");
	}
}
void TCPClient::SocketCallback(void* pDataLineHead)
{
	//处理数据
	SocketReadLine* pMsg = reinterpret_cast<SocketReadLine*>(pDataLineHead);
	void* pData = static_cast<char*>(pDataLineHead) + sizeof(SocketReadLine);

	unsigned int index = pMsg->uIndex;
	unsigned int size = pMsg->uHandleSize;
	const std::vector<TCPSocketInfo>& socketInfoVec = GetSocketVector();

	if (index >= 0 && index < socketInfoVec.size())
	{
		PlayerInfo Info;
		Info.pMsg = pMsg;
		Info.pData = pData;
		Info.uSrverType = GetServerType();

		static REvent eve;
		SocketReadLine* pMsg = reinterpret_cast<SocketReadLine*>(pDataLineHead);
		std::string pData = static_cast<char*>(pDataLineHead) + sizeof(SocketReadLine);

		Netmsg msg;
		msg << pMsg->netMessageHead.uMainID
			<< pMsg->netMessageHead.uAssistantID
			<< pMsg->netMessageHead.uIdentification
			<< pMsg->uHandleSize
			<< pData.c_str();

		memcpy(eve.m_Source, msg.str().c_str(), msg.str().size());

		m_NetworkCallBackFunc(eve);
	}
	else
	{
		Log(CERR, "Failed to process data，index=%d Out of range", index);
	}
}
void TCPClient::CloseSocketCallback(void* pDataLineHead)
{
	SocketCloseLine* pSocketClose = (SocketCloseLine*)pDataLineHead;
	if (!pSocketClose)
	{
		return;
	}
	m_CloseCallBackFunc();
}
