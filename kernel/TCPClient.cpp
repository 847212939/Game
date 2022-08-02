#include "../Game/stdafx.h"

TCPClient::TCPClient() : m_PlayerPrepClient(new PlayerPrepClient)
{
	RegisterNetType(this, TCPClient::SocketCallback, HD_SOCKET_READ);
	RegisterNetType(this, TCPClient::TimerCallback, HD_TIMER_MESSAGE);
	RegisterNetType(this, TCPClient::CloseSocketCallback, HD_SOCKET_CLOSE);
}

bool TCPClient::Init(ServiceType serverType)
{
	const LogicCfg& logicCfg = BaseCfgMgr.GetLogicCfg();
	int maxSocketCnt = BaseCfgMgr.GetMaxSocketCnt();

	if (!CTCPSocketManage::Init(maxSocketCnt, logicCfg.port, logicCfg.ip.c_str()))
	{
		return false;
	}
	if (!Start(serverType))
	{
		return false;
	}

	m_PlayerPrepClient->Init();

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
	if (m_PlayerPrepClient)
	{
		SafeDelete(m_PlayerPrepClient);
	}
}

void TCPClient::HandlerRecvDataListThread()
{
	CDataLine* pDataLine = GetRecvDataLine();
	if (!pDataLine)
	{
		COUT_LOG(LOG_CERROR, "CDataLine error pDataLine == nullptr");
		return;
	}
	if (!GetRuninged())
	{
		COUT_LOG(LOG_CERROR, "TCPClient::HandlerRecvDataListThread 初始化未完成");
		return;
	}

	void* pDataLineHead = nullptr;
	bool& run = GetRuninged();

	while (run)
	{
		unsigned int uDataKind = 0;
		unsigned int bytes = pDataLine->GetData(&pDataLineHead, run, uDataKind);
		if (bytes == 0 || pDataLineHead == nullptr || uDataKind <= 0)
		{
			continue;
		}
		
		CallBackFun(uDataKind, pDataLineHead);

		if (pDataLineHead)
		{
			SafeDeleteArray(pDataLineHead);
		}
	}

	COUT_LOG(LOG_CINFO, "recv data thread end...");

	return;
}

PlayerPrepClient* TCPClient::GetPlayerPrepClient()
{
	return m_PlayerPrepClient;
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
	CServerTimer* pCServerTimer = DPlayerPrepClient->GetCServerTimer();
	if (!pCServerTimer)
	{
		COUT_LOG(LOG_CERROR, "pCServerTimer = null");
		return;
	}
	int timerCnt = BaseCfgMgr.GetTimerCnt();

	RecvDataLine->GetConditionVariable().NotifyAll();
	SendDataLine->GetConditionVariable().NotifyAll();
	DPlayerPrepClient->GetConditionVariable().NotifyAll();
	DPlayerCenterClient->GetConditionVariable().NotifyAll();

	for (int i = 0; i < timerCnt; i++)
	{
		pCServerTimer[i].SetTimerRun(false);
	}
}

void TCPClient::AddNetTypeCallback(int cmd, std::function<void(void* pDataLineHead)>&& fun)
{
	TypeFunMap::iterator it = m_TypeFunMap.find(cmd);
	if (it == m_TypeFunMap.end())
	{
		m_TypeFunMap.insert(std::make_pair(cmd, fun));
		return;
	}

	COUT_LOG(LOG_CINFO, "There is already a callback for this message. Please check the code cmd = %d", cmd);
}

bool TCPClient::CallBackFun(unsigned int cmd, void* pDataLineHead)
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

void TCPClient::TimerCallback(void* pDataLineHead)
{
	ServerTimerLine* WindowTimer = (ServerTimerLine*)pDataLineHead;
	if (WindowTimer->uMainID == (unsigned int)MsgCmd::MsgCmd_Timer)
	{
		m_PlayerPrepClient->CallBackFun((TimerCmd)WindowTimer->uTimerID);
	}
	else
	{
		COUT_LOG(LOG_CERROR, "Timer message error");
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
		m_PlayerPrepClient->HandlerMessage(&Info);
	}
	else
	{
		COUT_LOG(LOG_CERROR, "Failed to process data，index=%d Out of range", index);
	}
}

void TCPClient::CloseSocketCallback(void* pDataLineHead)
{
	SocketCloseLine* pSocketClose = (SocketCloseLine*)pDataLineHead;

	PlayerClient* playerClient = DPlayerCenterClient->GetPlayerClientByIndex(pSocketClose->uIndex);
	if (!playerClient)
	{
		COUT_LOG(LOG_CINFO, "TCP close playerClient is null index = %d", pSocketClose->uIndex);
		return;
	}

	playerClient->ExitGame(pSocketClose);

	SafeDelete(playerClient);
}

