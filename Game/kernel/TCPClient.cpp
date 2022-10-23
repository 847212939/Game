#include "../stdafx.h"

TCPClient::TCPClient() : CTCPSocketManage(), m_PlayerPrepClient(new PlayerPrepClient)
{
	RegisterNetType(TCPClient::SocketCallback, SysMsgCmd::HD_SOCKET_READ);
	RegisterNetType(TCPClient::TimerCallback, SysMsgCmd::HD_TIMER_MESSAGE);
	RegisterNetType(TCPClient::CloseSocketCallback, SysMsgCmd::HD_SOCKET_CLOSE);
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
bool TCPClient::InitDBServer()
{
	const CLogicCfg& DBserverCfg = BaseCfgMgr.GetDBServerCfg();

	SOCKFD sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
	{
		COUT_LOG(LOG_CINFO, "连接服DBServer失败");
		return false;
	}

	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(DBserverCfg.port);

#if defined(_WIN32)
	sin.sin_addr.S_un.S_addr = inet_addr(DBserverCfg.ip.c_str());
#elif defined(_WIN64)
#elif defined(__linux__)
	sin.sin_addr.s_addr = inet_addr(m_ip.c_str());
#elif defined(__unix__)
#elif defined(__ANDROID__)
#elif defined(__APPLE__)
#endif

	if (connect(sock, (sockaddr*)&sin, sizeof(sockaddr_in)) < 0)
	{
		COUT_LOG(LOG_CINFO, "连接服DBServer失败");
		return false;
	}
	int index = GetSocketIndex();
	if (index < 0)
	{
		COUT_LOG(LOG_CINFO, "连接服DBServer失败");
		return false;
	}
	// 获取连接信息
	PlatformSocketInfo tcpInfo;
	tcpInfo.acceptMsgTime = time(nullptr);
	memcpy(tcpInfo.ip, DBserverCfg.ip.c_str(), DBserverCfg.ip.size());
	tcpInfo.port = DBserverCfg.port;
	tcpInfo.acceptFd = sock;	//服务器accept返回套接字用来和客户端通信

	AddTCPSocketInfo(index, &tcpInfo);
	COUT_LOG(LOG_CINFO, "连接服DBServer成功");
}
bool TCPClient::Init(ServiceType serverType)
{
	const CLogicCfg& logicCfg = BaseCfgMgr.GetLogicCfg();
	int maxSocketCnt = BaseCfgMgr.GetMaxSocketCnt();

	if (!CTCPSocketManage::Init(maxSocketCnt, logicCfg.port, logicCfg.ip.c_str(), serverType))
	{
		return false;
	}
	if (!Start())
	{
		return false;
	}

	m_PlayerPrepClient->Init();

	GetSockeThreadVec().push_back(new std::thread(&TCPClient::HandlerRecvDataListThread, this));

	COUT_LOG(LOG_CINFO, "Server initialization succeeded");
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
	CServerTimer* pCServerTimer = DPPC->GetCServerTimer();
	if (!pCServerTimer)
	{
		COUT_LOG(LOG_CERROR, "pCServerTimer = null");
		return;
	}
	int timerCnt = BaseCfgMgr.GetTimerCnt();

	RecvDataLine->GetConditionVariable().NotifyAll();
	SendDataLine->GetConditionVariable().NotifyAll();
	DPPC->GetConditionVariable().NotifyAll();
	DPCC->GetConditionVariable().NotifyAll();

	for (int i = 0; i < timerCnt; i++)
	{
		pCServerTimer[i].SetTimerRun(false);
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

	COUT_LOG(LOG_CINFO, "There is already a callback for this message. Please check the code cmd = %d", cmd);
}
bool TCPClient::CallBackFun(SysMsgCmd cmd, void* pDataLineHead)
{
	MapTypeFunc::iterator it = m_TypeFunMap.find(cmd);
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
		m_PlayerPrepClient->MessageDispatch(&Info);
	}
	else
	{
		COUT_LOG(LOG_CERROR, "Failed to process data，index=%d Out of range", index);
	}
}
void TCPClient::CloseSocketCallback(void* pDataLineHead)
{
	SocketCloseLine* pSocketClose = (SocketCloseLine*)pDataLineHead;

	PlayerClient* playerClient = DPCC->GetPlayerClientByIndex(pSocketClose->uIndex);
	if (!playerClient)
	{
		COUT_LOG(LOG_CINFO, "TCP close playerClient is null index = %d", pSocketClose->uIndex);
		return;
	}

	playerClient->ExitGame(pSocketClose);

	SafeDelete(playerClient);
}